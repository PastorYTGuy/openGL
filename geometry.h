#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QOpenGLFunctions_4_2_Core>
#include <QMap>
#include "utils.h"
#include "glm/glm.hpp"

// Klasa zarzadzajaca geometria, buforami i renderingiem
class Geometry : protected QOpenGLFunctions_4_2_Core
{
protected:
    GLuint vao;    //Vertex Array Object
    int n_verts;   //ilosc wierzcholkow

    GLuint ibo;     //Index Bufor Object
    int n_indices;  //ilosc indeksow

    //! Mapa buforow atrybutow
    //! klucz mapy to index danego atrybutu, wartosc mapy to uchwyt do bufora
    QMap<uint, GLuint> bufferObjects;
    GLenum primitiveMode;  //tryb rysowania geometrii: punkty, linie, trojkaty

public:        
    Geometry();
    virtual ~Geometry();

    void setPrimitiveMode(GLenum);
    virtual void render();

    void setIndices(uint* data, int n);

    void setVertices(uint index, glm::vec3* verts, int n);
    void setAttribute(uint index, glm::vec3* data, int n);

    void setAttribute(uint index, glm::vec2* data, int n);
    void setAttribute(uint index, float* data, int n);
};



#endif // GEOMETRY_H
