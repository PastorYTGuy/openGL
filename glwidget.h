#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "glslprogram.h"
#include "geometry.h"

#if QT_VERSION_MAJOR >= 6
    #include <QtOpenGLWidgets/QOpenGLWidget>
#else
    #include <QOpenGLWidget>
#endif

#include <QOpenGLFunctions_4_2_Core>
#include <QOpenGLDebugLogger>
#include <QMap>
#include <QDebug>
#include <QTimer>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <QOpenGLTexture>

struct Frame {
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scale    = glm::vec3(1.0f);

    glm::mat4 getModelMatrix() const {
        glm::mat4 mat = glm::mat4(1.0f);
        mat = glm::translate(mat, position);
        mat = glm::rotate(mat, glm::radians(rotation.x), glm::vec3(1, 0, 0));
        mat = glm::rotate(mat, glm::radians(rotation.y), glm::vec3(0, 1, 0));
        mat = glm::rotate(mat, glm::radians(rotation.z), glm::vec3(0, 0, 1));
        mat = glm::scale(mat, scale);
        return mat;
    }
};

struct Camera {
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 2.0f);
    float ud = 0.0f;
    float lr = -90.0f;

    glm::mat4 getViewMatrix() const {
        return glm::lookAt(position, position + getForwardVector(), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::vec3 getForwardVector() const {
        glm::vec3 forward;
        //Konwersja kątów Eulera na wektor
        forward.x = cos(glm::radians(lr)) * cos(glm::radians(ud));
        forward.y = sin(glm::radians(ud));
        forward.z = sin(glm::radians(lr)) * cos(glm::radians(ud));
        return glm::normalize(forward);
    }

    glm::vec3 getRightVector() const {
        return glm::normalize(glm::cross(getForwardVector(), glm::vec3(0, 1, 0)));
    }
};


class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_2_Core
{
    QMap<QString, GLSLProgram*> shaders;
    QMap<QString, Geometry*> geometry;
    QMap<QString, glm::mat4> geometryMat;

    glm::mat4 MVMat;
    glm::mat4 ProjMat;

    QOpenGLDebugLogger *m_logger = nullptr;
    QTimer frame_timer;
    int frame;

    void createShaders();
    void createGeometry();

    int pos_x, pos_y;
    float zoom;

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);


    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event) override;

    void processInput();
    QMap<int, bool> keys;

    Camera camera;
    QPoint lastMousePos;
    Frame cylinderFrame;

    QOpenGLTexture* heightMapTexture = nullptr;
    QOpenGLTexture* metalTexture = nullptr;
    QOpenGLTexture* grassTexture = nullptr;
    QOpenGLTexture* metalNormalMap = nullptr;

    QOpenGLTexture* skyboxTexture = nullptr;
    QOpenGLTexture* treeTexture = nullptr;
    Geometry* skyboxGeometry = nullptr;
    Geometry* treeGeometry = nullptr;
    GLuint skyboxTextureID;
public:
    GLWidget();
    ~GLWidget();
};


#endif // GLWIDGET_H
