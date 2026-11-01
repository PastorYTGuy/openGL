#ifndef GLSLPROGRAM_H
#define GLSLPROGRAM_H

//#ifdef __linux
//#define GL_GLEXT_PROTOTYPES 1
//#endif

#include <QOpenGLFunctions_4_2_Core>
#include <string>
#include <QDebug>

#include "glm/glm.hpp"
#include "utils.h"


//Klasa zarzadzajaca programem shadera
class GLSLProgram :  protected QOpenGLFunctions_4_2_Core
{
    GLuint handle;
    bool checkShaderStatus(GLuint);
    bool checkProgramStatus(GLuint);

public:
    GLSLProgram();
    ~GLSLProgram();
    bool compileShaderFromFile(const char* filename, GLenum type);
    bool compileShaderFromString(const std::string& source, GLenum type);
    bool link();
    void use();    

    void setUniform(const char* name, glm::vec3 v);
    void setUniform(const char* name, glm::mat4 mat);

    void setUniform(const char* name, int v);
    void setUniform(const char* name, float v);
};

#endif // GLSLPROGRAM_H
