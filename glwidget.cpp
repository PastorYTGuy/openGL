#include "glwidget.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <glm/ext/matrix_clip_space.hpp>
#include "glm/glm.hpp"
#include "glm/common.hpp"
#include <glm/ext/matrix_transform.hpp>
#include "primitives.h"

GLWidget::GLWidget()
{
    frame = 0;
    pos_x = pos_y = 0;
    zoom = 1.0f;
    camera.position = glm::vec3(0.0f, 1.5f, 5.0f);

    cylinderFrame.position = glm::vec3(0.0f, 0.001f, 0.0f);
    cylinderFrame.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    cylinderFrame.scale    = glm::vec3(1.0f, 1.0f, 1.0f);
}

GLWidget::~GLWidget()
{
    qDeleteAll(shaders);
    qDeleteAll(geometry);
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    setFocusPolicy(Qt::StrongFocus);

    m_logger = new QOpenGLDebugLogger(this);
    if (m_logger->initialize()) {
        connect(m_logger, &QOpenGLDebugLogger::messageLogged, this, [](const QOpenGLDebugMessage &msg) {
            if (msg.id() == 131185 || msg.id() == 131218)
                return;
            qDebug() << msg;
        });
        m_logger->startLogging();
    }

    createShaders();

    QImage image("res/clouds.png");
    if (image.isNull()) {
        qFatal("Nie udalo sie wczytac pliku PNG z mapa wysokosci!");
    }
    heightMapTexture = new QOpenGLTexture(image);
    heightMapTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    heightMapTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    heightMapTexture->setWrapMode(QOpenGLTexture::ClampToEdge);

    QImage imgMetal("res/metal.jpg");
    if (!imgMetal.isNull()) {
        metalTexture = new QOpenGLTexture(imgMetal);
        metalTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        metalTexture->setMagnificationFilter(QOpenGLTexture::Linear);
        metalTexture->setWrapMode(QOpenGLTexture::Repeat);
    } else {
        qFatal("Nie udalo sie wczytac tekstury metalu!");
    }

    QImage imgNormal("res/metal_normal.jpg");
    if (!imgNormal.isNull()) {
        metalNormalMap = new QOpenGLTexture(imgNormal);
        metalNormalMap->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        metalNormalMap->setMagnificationFilter(QOpenGLTexture::Linear);
        metalNormalMap->setWrapMode(QOpenGLTexture::Repeat);
    } else {
        qFatal("Nie udalo sie wczytac normal mapy metalu!");
    }

    QImage imgGrass("res/grass.jpg");
    if (!imgGrass.isNull()) {
        grassTexture = new QOpenGLTexture(imgGrass);
        grassTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        grassTexture->setMagnificationFilter(QOpenGLTexture::Linear);
        grassTexture->setWrapMode(QOpenGLTexture::Repeat);
    } else {
        qFatal("Nie udalo sie wczytac tekstury trawy!");
    }

    glGenTextures(1, &skyboxTextureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);

    QStringList faces = {"sky_px", "sky_nx", "sky_py", "sky_ny", "sky_pz", "sky_nz"};
    for(int i = 0; i < 6; ++i) {
        QImage img("res/" + faces[i]);
        if (img.isNull()) img.load("res/" + faces[i] + ".png");

        QImage glImage = img.convertToFormat(QImage::Format_RGBA8888);

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8,
                     glImage.width(), glImage.height(), 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, glImage.constBits());
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    QImage imgTree("res/tree.png");
    if (!imgTree.isNull()) {
        treeTexture = new QOpenGLTexture(imgTree.convertToFormat(QImage::Format_RGBA8888));
        treeTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        treeTexture->setMagnificationFilter(QOpenGLTexture::Linear);
        treeTexture->setWrapMode(QOpenGLTexture::ClampToEdge);
    } else {
        qFatal("Nie udalo sie wczytac tekstury drzewa!");
    }

    treeGeometry = newTexturedPlaneGeometry(glm::vec2(1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

    createGeometry();

    connect(&frame_timer, &QTimer::timeout, this, QOverload<>::of(&GLWidget::update));
    frame_timer.setInterval(10);
    frame_timer.start();
}

void GLWidget::createShaders()
{
    shaders["basic"] = new GLSLProgram;
    bool stat = shaders["basic"]->compileShaderFromFile("shaders/vs_ADS_phong.glsl", GL_VERTEX_SHADER);
    stat &= shaders["basic"]->compileShaderFromFile("shaders/fs_ADS_phong.glsl", GL_FRAGMENT_SHADER);
    stat &= shaders["basic"]->link();

    shaders["skybox"] = new GLSLProgram;
    stat &= shaders["skybox"]->compileShaderFromFile("shaders/vs_skybox.glsl", GL_VERTEX_SHADER);
    stat &= shaders["skybox"]->compileShaderFromFile("shaders/fs_skybox.glsl", GL_FRAGMENT_SHADER);
    stat &= shaders["skybox"]->link();

    if (!stat) qFatal("Some problem with shader!");
}

void GLWidget::createGeometry()
{
    geometry["main_axes"] = newAxesGeometry();
    geometryMat["main_axes"] = glm::mat4(1);

    geometry["plane"] = newTerrainGridGeometry(512);
    geometryMat["plane"] = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));

    geometry["cylinder"] = newCylinderGeometry(0.6f, 2.0f, 8, glm::vec3(0.2f, 0.6f, 1.0f));
    geometryMat["cylinder"] = glm::mat4(1.0f);

    geometry["box"] = newBoxGeometry({0.5f, 0.5f, 0.5f}, {0, 1, 0});
    geometryMat["box"] = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));

    geometry["circ"] = newCircleGeometry(0.75f, 24, {1, 0, 0});
    geometryMat["circ"] = glm::mat4(1.0f);
}

void GLWidget::mousePressEvent(QMouseEvent *e)
{
    lastMousePos = e->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton) {
        QPoint delta = e->pos() - lastMousePos;
        lastMousePos = e->pos();

        float sensitivity = 0.15f;

        camera.lr += delta.x() * sensitivity;
        camera.ud -= delta.y() * sensitivity;
        camera.ud = glm::clamp(camera.ud, -89.0f, 89.0f);

        update();
    }
}

void GLWidget::keyPressEvent(QKeyEvent *e)
{
    if (!e->isAutoRepeat()) {
        keys[e->key()] = true;
    }
}

void GLWidget::keyReleaseEvent(QKeyEvent *e)
{
    if (!e->isAutoRepeat()) {
        keys[e->key()] = false;
    }
}

void GLWidget::processInput()
{
    float cameraSpeed = 0.03f;
    glm::vec3 forward = camera.getForwardVector();
    glm::vec3 right   = camera.getRightVector();

    if (keys[Qt::Key_W]) camera.position += forward * cameraSpeed;
    if (keys[Qt::Key_S]) camera.position -= forward * cameraSpeed;
    if (keys[Qt::Key_A]) camera.position -= right * cameraSpeed;
    if (keys[Qt::Key_D]) camera.position += right * cameraSpeed;

    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    if (keys[Qt::Key_Space]) {
        camera.position += up * cameraSpeed;
    }

    if (keys[Qt::Key_Control]) {
        camera.position -= up * cameraSpeed;
    }
}

void GLWidget::wheelEvent(QWheelEvent *e)
{
    zoom += e->angleDelta().y() / 10000.0f;
    zoom = glm::clamp(zoom, 0.1f, 10.0f);
    update();
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    ProjMat = glm::perspective(glm::radians(45.0f), (float)w / (float)h, 0.1f, 100.0f);
}

void GLWidget::paintGL()
{
    processInput();

    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glm::mat4 identity = glm::mat4(1.0f);
    glm::mat4 view = camera.getViewMatrix();
    view = glm::scale(view, glm::vec3(zoom, zoom, zoom));

    glm::vec3 lightPosition = glm::vec3(7.0f, 5.5f, 0.0f);

    if (shaders.contains("skybox") && skyboxTextureID != 0) {
        glDisable(GL_CULL_FACE);
        glDepthMask(GL_FALSE);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        shaders["skybox"]->use();
        shaders["skybox"]->setUniform("ProjMat", ProjMat);
        glm::mat4 skyboxView = glm::mat4(glm::mat3(camera.getViewMatrix()));
        shaders["skybox"]->setUniform("ViewMat", skyboxView);

        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
        shaders["skybox"]->setUniform("skybox", 4);

        if (geometry.contains("box")) {
            geometry["box"]->render();
        }
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
    }

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    if (shaders.contains("basic")) {
        shaders["basic"]->use();

        shaders["basic"]->setUniform("ProjMat", ProjMat);
        shaders["basic"]->setUniform("ViewMat", view);
        shaders["basic"]->setUniform("LightPos", lightPosition);
        shaders["basic"]->setUniform("ViewPos", camera.position);
        shaders["basic"]->setUniform("LightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        shaders["basic"]->setUniform("fogColor", glm::vec3(0.5f, 0.5f, 0.6f));
        shaders["basic"]->setUniform("fogStart", 2.0f);
        shaders["basic"]->setUniform("fogEnd", 15.0f);

        if (heightMapTexture && heightMapTexture->isCreated()) {
            glActiveTexture(GL_TEXTURE0);
            heightMapTexture->bind();
            shaders["basic"]->setUniform("heightMap", 0);
            shaders["basic"]->setUniform("heightScale", 0.1f);
        }

        if (grassTexture && grassTexture->isCreated()) {
            glActiveTexture(GL_TEXTURE3);
            grassTexture->bind();
            shaders["basic"]->setUniform("grassMap", 3);
            shaders["basic"]->setUniform("useNormalMap", 2);
        } else {
            shaders["basic"]->setUniform("useNormalMap", 0);
        }

        glm::mat4 groundMat = glm::scale(identity, glm::vec3(5.0f, 1.0f, 5.0f));
        shaders["basic"]->setUniform("ModelMat", groundMat);
        shaders["basic"]->setUniform("ObjectColor", glm::vec3(1.0f, 1.0f, 1.0f));
        if (geometry.contains("plane")) {
            geometry["plane"]->render();
        }

        shaders["basic"]->setUniform("heightScale", 0.0f);

        if (metalTexture && metalTexture->isCreated()) {
            glActiveTexture(GL_TEXTURE1);
            metalTexture->bind();
            shaders["basic"]->setUniform("colorMap", 1);
        }
        if (metalNormalMap && metalNormalMap->isCreated()) {
            glActiveTexture(GL_TEXTURE2);
            metalNormalMap->bind();
            shaders["basic"]->setUniform("normalMap", 2);
            shaders["basic"]->setUniform("useNormalMap", 1);
        } else {
            shaders["basic"]->setUniform("useNormalMap", 0);
        }

        glm::mat4 cylinderMat = identity;
        cylinderMat = glm::translate(cylinderMat, glm::vec3(0.0f, 0.01f, 0.0f));
        cylinderMat = glm::scale(cylinderMat, glm::vec3(0.3f, 0.6f, 0.3f));
        shaders["basic"]->setUniform("ModelMat", cylinderMat);
        shaders["basic"]->setUniform("ObjectColor", glm::vec3(1.0f, 1.0f, 1.0f));
        if (geometry.contains("cylinder")) {
            geometry["cylinder"]->render();
        }

        if (treeTexture && treeTexture->isCreated() && treeGeometry) {
            glDisable(GL_CULL_FACE);
            glActiveTexture(GL_TEXTURE5);
            treeTexture->bind();
            shaders["basic"]->setUniform("treeMap", 5);
            shaders["basic"]->setUniform("useNormalMap", 3);

            glm::mat4 treeMat = identity;
            treeMat = glm::translate(treeMat, glm::vec3(1.5f, 1.0f, 1.5f));
            treeMat = glm::rotate(treeMat, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            shaders["basic"]->setUniform("ModelMat", treeMat);
            shaders["basic"]->setUniform("ObjectColor", glm::vec3(1.0f, 1.0f, 1.0f));
            treeGeometry->render();
            glEnable(GL_CULL_FACE);
        }

        shaders["basic"]->setUniform("useNormalMap", 0);

        glm::mat4 axesMat = glm::translate(identity, glm::vec3(0.0f, 0.01f, 0.0f));
        shaders["basic"]->setUniform("ModelMat", axesMat);
        if (geometry.contains("main_axes")) {
            geometry["main_axes"]->render();
        }
    }

    glDisable(GL_CULL_FACE);
    frame++;
}