QT += gui opengl widgets
CONFIG += debug c++11

equals(QT_MAJOR_VERSION, 6) {
    QT += openglwidgets
}

SOURCES += \
    geometry.cpp \
    main.cpp \
    glwidget.cpp \
    glslprogram.cpp \
    primitives.cpp

HEADERS += \
    geometry.h \
    glwidget.h \
    glslprogram.h \
    primitives.h \
    utils.h


win32 {
    LIBS += -lopengl32
}

DISTFILES += \
    shaders/fs.glsl \
    shaders/vs.glsl \
    shaders/vs_skybox.glsl \
    shaders/fs_skybox.glsl \
    shaders/vs_ADS_phong.glsl \
    shaders/fs_ADS_phong.glsl