#include "primitives.h"

Geometry *newAxesGeometry()
{
    Geometry* geometry = new Geometry();
    {
        geometry->setPrimitiveMode(GL_LINES);
        glm::vec3 verts[] = { {0,0,0}, {1.0,0,0}, {0,0,0}, {0,1.0,0}, {0,0,0}, {0,0,1.0} };
        glm::vec3 colors[] = { {1,0,0}, {1,0,0},  {0,1,0}, {0,1,0},   {0,0,1}, {0,0,1} };
        geometry->setVertices((int)Attributes::position, verts, 6);
        geometry->setAttribute((int)Attributes::color, colors, 6);
    }
    return geometry;
}

Geometry *newPlaneGeometry(glm::vec2 size, glm::vec3 color)
{
    Geometry* geometry = new Geometry();
    {
        geometry->setPrimitiveMode(GL_TRIANGLES);
        glm::vec3 verts[]  = { {-size.x,-size.y,0}, {-size.x,size.y,0}, {size.x,size.y,0}, {size.x,-size.y,0}};
        glm::vec3 colors[] = { color, color, color, color};
        uint indices[] = {0,1,2,2,3,0};
        geometry->setIndices(indices, 6);
        geometry->setVertices((int)Attributes::position, verts, 4);
        geometry->setAttribute((int)Attributes::color, colors, 4);
    }
    return geometry;
}

Geometry* newTexturedPlaneGeometry(glm::vec2 size, glm::vec3 color)
{
    Geometry* geometry = new Geometry();
    geometry->setPrimitiveMode(GL_TRIANGLES);

    glm::vec3 verts[] = {
        {-size.x, -size.y, 0.0f},
        {-size.x,  size.y, 0.0f},
        { size.x,  size.y, 0.0f},
        { size.x, -size.y, 0.0f}
    };

    glm::vec2 texCoords[] = {
        {0.0f, 0.0f},
        {0.0f, 1.0f},
        {1.0f, 1.0f},
        {1.0f, 0.0f}
    };

    glm::vec3 normals[] = {
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f}
    };

    glm::vec3 colors[] = { color, color, color, color };
    uint indices[] = { 0, 1, 2, 2, 3, 0 };

    geometry->setIndices(indices, 6);
    geometry->setVertices((int)Attributes::position, verts, 4);
    geometry->setAttribute(1, texCoords, 4);
    geometry->setAttribute(2, normals, 4);
    geometry->setAttribute((int)Attributes::color, colors, 4);

    return geometry;
}

Geometry* newBoxGeometry(glm::vec3 size, glm::vec3 color)
{
    Geometry* geometry = new Geometry();
    geometry->setPrimitiveMode(GL_TRIANGLES);
    {
        int n = 24;
        glm::vec3 origin = {0.0, 0.0, 0.0};
        float w = size.x;
        float h = size.y;
        float d = size.z;

        std::vector<glm::vec3> pos(n);
        std::vector<glm::vec3> colors(n);
        std::vector<uint> indices(36);

        int it=0;
        //front
        pos[it++] = origin + glm::vec3{-w,  h, d};
        pos[it++] = origin + glm::vec3{-w, -h, d};
        pos[it++] = origin + glm::vec3{ w, -h, d};
        pos[it++] = origin + glm::vec3{ w,  h, d};

        //right
        pos[it++] = origin + glm::vec3{w,  h, d};
        pos[it++] = origin + glm::vec3{w, -h, d};
        pos[it++] = origin + glm::vec3{w, -h, -d};
        pos[it++] = origin + glm::vec3{w,  h, -d};

        //back
        pos[it++] = origin + glm::vec3{-w,  h, -d};
        pos[it++] = origin + glm::vec3{-w, -h, -d};
        pos[it++] = origin + glm::vec3{ w, -h, -d};
        pos[it++] = origin + glm::vec3{ w,  h, -d};

        //left
        pos[it++] = origin + glm::vec3{-w,  h, d};
        pos[it++] = origin + glm::vec3{-w, -h, d};
        pos[it++] = origin + glm::vec3{-w, -h, -d};
        pos[it++] = origin + glm::vec3{-w,  h, -d};

        //top
        pos[it++] = origin + glm::vec3{-w, h, -d};
        pos[it++] = origin + glm::vec3{-w, h,  d};
        pos[it++] = origin + glm::vec3{ w, h,  d};
        pos[it++] = origin + glm::vec3{ w, h, -d};

        //bottom
        pos[it++] = origin + glm::vec3{-w, -h, -d};
        pos[it++] = origin + glm::vec3{-w, -h,  d};
        pos[it++] = origin + glm::vec3{ w, -h,  d};
        pos[it++] = origin + glm::vec3{ w, -h, -d};

        for(int i=0; i<n; i++)
            colors[i] = color;
        geometry->setVertices((int)Attributes::position, pos.data(), n);
        geometry->setAttribute((int)Attributes::color, colors.data(), n);

        it=0;
        indices[it++] = 0;    indices[it++] = 1;    indices[it++] = 2;    indices[it++] = 0;    indices[it++] = 2;    indices[it++] = 3;
        indices[it++] = 4;    indices[it++] = 5;    indices[it++] = 6;    indices[it++] = 4;    indices[it++] = 6;    indices[it++] = 7;
        indices[it++] = 8;    indices[it++] = 11;   indices[it++] = 10;   indices[it++] = 8;    indices[it++] = 10;   indices[it++] = 9;
        indices[it++] = 12;   indices[it++] = 15;   indices[it++] = 14;   indices[it++] = 12;   indices[it++] = 14;   indices[it++] = 13;
        indices[it++] = 16;   indices[it++] = 17;   indices[it++] = 18;   indices[it++] = 16;   indices[it++] = 18;   indices[it++] = 19;
        indices[it++] = 20;   indices[it++] = 23;   indices[it++] = 22;   indices[it++] = 20;   indices[it++] = 22;   indices[it++] = 21;

        geometry->setIndices(indices.data(), indices.size());
    }
    return geometry;
}

Geometry* newCircleGeometry(float radius, int n_segments, glm::vec3 color)
{
    Geometry* geometry = new Geometry();
    {
        geometry->setPrimitiveMode(GL_TRIANGLES);
        std::vector<glm::vec3> verts;
        std::vector<glm::vec3> colors;
        std::vector<uint> indices;

        verts.push_back({0,0,0});
        colors.push_back(color);
        float a=0.0f;

        for(int i=0; i<n_segments; i++){
            float x = radius * cos(a);
            float y = radius * sin(a);
            verts.push_back({x,y,0});
            colors.push_back(color);
            a += 2*M_PI/n_segments;
        }

        for(int i=0; i<n_segments; i++){
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i+1);
        }
        indices.push_back(0);
        indices.push_back(n_segments);
        indices.push_back(1);


        geometry->setIndices(indices.data(), indices.size());
        geometry->setVertices((int)Attributes::position, verts.data(), verts.size());
        geometry->setAttribute((int)Attributes::color, colors.data(), colors.size());
    }
    return geometry;
}

Geometry* newCylinderGeometry(float radius, float height, int n_segments, glm::vec3 color)
{
    Geometry* geometry = new Geometry();
    geometry->setPrimitiveMode(GL_TRIANGLES);

    std::vector<glm::vec3> verts;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> tangents;
    std::vector<uint> indices;

    verts.push_back({0.0f, 0.0f, 0.0f});
    texCoords.push_back({0.5f, 0.5f});
    normals.push_back({0.0f, -1.0f, 0.0f});
    tangents.push_back({1.0f, 0.0f, 0.0f});

    verts.push_back({0.0f, height, 0.0f});
    texCoords.push_back({0.5f, 0.5f});
    normals.push_back({0.0f, 1.0f, 0.0f});
    tangents.push_back({1.0f, 0.0f, 0.0f});

    float a = 0.0f;
    for(int i = 0; i <= n_segments; i++){
        float x = radius * cos(a);
        float z = radius * sin(a);

        glm::vec3 side_normal = glm::normalize(glm::vec3(x, 0.0f, z));
        glm::vec3 side_tangent = glm::normalize(glm::vec3(-sin(a), 0.0f, cos(a)));

        float u = (float)i / n_segments;

        verts.push_back({x, 0.0f, z});
        texCoords.push_back({u, 0.0f});
        normals.push_back(side_normal);
        tangents.push_back(side_tangent);

        verts.push_back({x, height, z});
        texCoords.push_back({u, 1.0f});
        normals.push_back(side_normal);
        tangents.push_back(side_tangent);

        a += 2.0f * M_PI / n_segments;
    }

    for(int i = 0; i < n_segments; i++){
        int current_bot = 2 + 2 * i;
        int current_top = 2 + 2 * i + 1;
        int next_bot = 2 + 2 * (i + 1);
        int next_top = 2 + 2 * (i + 1) + 1;

        indices.push_back(next_bot);
        indices.push_back(current_bot);
        indices.push_back(current_top);

        indices.push_back(current_top);
        indices.push_back(next_top);
        indices.push_back(next_bot);

        indices.push_back(0);
        indices.push_back(current_bot);
        indices.push_back(next_bot);

        indices.push_back(1);
        indices.push_back(next_top);
        indices.push_back(current_top);
    }

    geometry->setIndices(indices.data(), indices.size());
    geometry->setVertices((int)Attributes::position, verts.data(), verts.size());
    geometry->setAttribute(1, texCoords.data(), texCoords.size());
    geometry->setAttribute(2, normals.data(), normals.size());
    geometry->setAttribute(3, tangents.data(), tangents.size());

    return geometry;
}

Geometry* newTerrainGridGeometry(int resolution)
{
    Geometry* geometry = new Geometry();
    geometry->setPrimitiveMode(GL_TRIANGLES);

    std::vector<glm::vec3> verts;
    std::vector<glm::vec3> texCoords;
    std::vector<uint> indices;

    for (int z = 0; z <= resolution; z++) {
        for (int x = 0; x <= resolution; x++) {
            float xPos = (float)x / resolution * 2.0f - 1.0f;
            float zPos = (float)z / resolution * 2.0f - 1.0f;
            float u = (float)x / resolution;
            float v = (float)z / resolution;

            verts.push_back({xPos, 0.0f, zPos});
            texCoords.push_back({u, v, 0.0f});
        }
    }

    for (int z = 0; z < resolution; z++) {
        for (int x = 0; x < resolution; x++) {
            int row1 = z * (resolution + 1);
            int row2 = (z + 1) * (resolution + 1);

            indices.push_back(row1 + x);
            indices.push_back(row2 + x);
            indices.push_back(row1 + x + 1);

            indices.push_back(row1 + x + 1);
            indices.push_back(row2 + x);
            indices.push_back(row2 + x + 1);
        }
    }

    geometry->setIndices(indices.data(), indices.size());
    geometry->setVertices((int)Attributes::position, verts.data(), verts.size());
    geometry->setAttribute(1, texCoords.data(), texCoords.size());

    return geometry;
}
