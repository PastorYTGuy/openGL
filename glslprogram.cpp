#include "glslprogram.h"
#include <QDebug>
#include <QFile>


GLSLProgram::GLSLProgram()
{
    initializeOpenGLFunctions();
    handle = glCreateProgram();
}

GLSLProgram::~GLSLProgram()
{
    if (handle != 0) {
        glDeleteProgram(handle);
    }
}

void GLSLProgram::setUniform(const char* name, int v)
{
    GLint loc = glGetUniformLocation(handle, name);
    if(loc != -1) glUniform1i(loc, v);
}

void GLSLProgram::setUniform(const char* name, float v)
{
    GLint loc = glGetUniformLocation(handle, name);
    if(loc != -1) glUniform1f(loc, v);
}

bool GLSLProgram::compileShaderFromFile(const char *filename, GLenum type)
{
    QFile file(filename);
    if (file.open(QFile::ReadOnly))
    {
        std::string shader_code = file.readAll().toStdString();
        return compileShaderFromString(shader_code, type);
    }
    else
    {
        qDebug() << "WARNING: GLSLProgram::compileShaderFromFile() brak pliku shadera " << filename;
    }
    return false;
}

bool GLSLProgram::compileShaderFromString(const std::string &source, GLenum type)
{
    GLuint sh = glCreateShader(type);
    const char* srcs[] = {source.c_str()};
    glShaderSource(sh, 1, srcs, NULL);
    glCompileShader(sh);
    if( checkShaderStatus(sh) )
    {
        glAttachShader(handle, sh);
        glDeleteShader(sh);
        return true;
    }
    glDeleteShader(sh);
    return false;
}

bool GLSLProgram::link()
{
    glLinkProgram(handle);
    return checkProgramStatus(handle);
}

void GLSLProgram::use()
{
    glUseProgram(handle);
}

void GLSLProgram::setUniform(const char* name, glm::vec3 v)
{
    GLint loc = glGetUniformLocation(handle, name);
    if(loc != -1)
        glUniform3fv(loc, 1, &v.x);
}

void GLSLProgram::setUniform(const char *name, glm::mat4 mat)
{
    GLint loc = glGetUniformLocation(handle, name);
    if(loc != -1)
        glUniformMatrix4fv(loc, 1, GL_FALSE, &mat[0][0]);
}

bool GLSLProgram::checkShaderStatus(GLuint shader)
{
  GLint stat;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &stat);
  if(stat == GL_FALSE)
  {
    GLchar infoLog[10240];
    glGetShaderInfoLog(shader, 10240, NULL, infoLog);
    qDebug() << "Shader ERROR: " << infoLog;
    return false;
  }
  else{
        qDebug() << "Shader compiled " << shader;
  }
  return true;
}

bool GLSLProgram::checkProgramStatus(GLuint program)
{
  GLint stat;
  glGetProgramiv(program, GL_LINK_STATUS, &stat);
  if(stat == GL_FALSE)
  {
    GLchar infoLog[10240];
    glGetProgramInfoLog(program, 10240, NULL, infoLog);
    qDebug() << "Program ERROR: " << infoLog;
    return false;
  }
  else
  {
    qDebug() << "Program LINKED";
  }
  return true;
}
