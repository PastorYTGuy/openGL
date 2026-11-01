#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "geometry.h"

Geometry *newAxesGeometry();

Geometry *newPlaneGeometry(glm::vec2 size, glm::vec3 color);

Geometry* newBoxGeometry(glm::vec3 size, glm::vec3 color);

Geometry* newCircleGeometry(float radius, int n_segments, glm::vec3 color);

Geometry* newCylinderGeometry(float radius, float height, int n_segments, glm::vec3 color);

Geometry* newTerrainGridGeometry(int resolution);

Geometry* newTexturedPlaneGeometry(glm::vec2 size, glm::vec3 color);

#endif // PRIMITIVES_H
