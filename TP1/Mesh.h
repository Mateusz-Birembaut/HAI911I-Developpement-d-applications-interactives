#ifndef MESH_H
#define MESH_H

#include <vector>

struct Vec3 {
    float x {};
    float y {};
    float z {};
};

struct Vertex{
    Vec3 position;
    Vec3 normal;
};

class Mesh{

public:

    int getVerticesSize() const { return m_vertices.size();}
    int getIndexesSize() const { return m_indices.size();}

    const std::vector<Vertex>& getVertices() const { return m_vertices; }
    const std::vector<unsigned int>& getIndices() const { return m_indices; }

    void setupBuffers();

    Mesh()
        : m_vertices{
            // Front face (z = +0.2)
            {{-0.2f, -0.2f,  0.2f}, {0.0f, 0.0f, 1.0f}},
            {{ 0.2f, -0.2f,  0.2f}, {0.0f, 0.0f, 1.0f}},
            {{ 0.2f,  0.2f,  0.2f}, {0.0f, 0.0f, 1.0f}},
            {{-0.2f, -0.2f,  0.2f}, {0.0f, 0.0f, 1.0f}},
            {{ 0.2f,  0.2f,  0.2f}, {0.0f, 0.0f, 1.0f}},
            {{-0.2f,  0.2f,  0.2f}, {0.0f, 0.0f, 1.0f}},

            // Back face (z = -0.2)
            {{ 0.2f, -0.2f, -0.2f}, {0.0f, 0.0f, -1.0f}},
            {{-0.2f, -0.2f, -0.2f}, {0.0f, 0.0f, -1.0f}},
            {{-0.2f,  0.2f, -0.2f}, {0.0f, 0.0f, -1.0f}},
            {{ 0.2f, -0.2f, -0.2f}, {0.0f, 0.0f, -1.0f}},
            {{-0.2f,  0.2f, -0.2f}, {0.0f, 0.0f, -1.0f}},
            {{ 0.2f,  0.2f, -0.2f}, {0.0f, 0.0f, -1.0f}},

            // Left face (x = -0.2)
            {{-0.2f, -0.2f, -0.2f}, {-1.0f, 0.0f, 0.0f}},
            {{-0.2f, -0.2f,  0.2f}, {-1.0f, 0.0f, 0.0f}},
            {{-0.2f,  0.2f,  0.2f}, {-1.0f, 0.0f, 0.0f}},
            {{-0.2f, -0.2f, -0.2f}, {-1.0f, 0.0f, 0.0f}},
            {{-0.2f,  0.2f,  0.2f}, {-1.0f, 0.0f, 0.0f}},
            {{-0.2f,  0.2f, -0.2f}, {-1.0f, 0.0f, 0.0f}},

            // Right face (x = +0.2)
            {{ 0.2f, -0.2f,  0.2f}, {1.0f, 0.0f, 0.0f}},
            {{ 0.2f, -0.2f, -0.2f}, {1.0f, 0.0f, 0.0f}},
            {{ 0.2f,  0.2f, -0.2f}, {1.0f, 0.0f, 0.0f}},
            {{ 0.2f, -0.2f,  0.2f}, {1.0f, 0.0f, 0.0f}},
            {{ 0.2f,  0.2f, -0.2f}, {1.0f, 0.0f, 0.0f}},
            {{ 0.2f,  0.2f,  0.2f}, {1.0f, 0.0f, 0.0f}},

            // Top face (y = +0.2)
            {{-0.2f,  0.2f,  0.2f}, {0.0f, 1.0f, 0.0f}},
            {{ 0.2f,  0.2f,  0.2f}, {0.0f, 1.0f, 0.0f}},
            {{ 0.2f,  0.2f, -0.2f}, {0.0f, 1.0f, 0.0f}},
            {{-0.2f,  0.2f,  0.2f}, {0.0f, 1.0f, 0.0f}},
            {{ 0.2f,  0.2f, -0.2f}, {0.0f, 1.0f, 0.0f}},
            {{-0.2f,  0.2f, -0.2f}, {0.0f, 1.0f, 0.0f}},

            // Bottom face (y = -0.2)
            {{-0.2f, -0.2f, -0.2f}, {0.0f, -1.0f, 0.0f}},
            {{ 0.2f, -0.2f, -0.2f}, {0.0f, -1.0f, 0.0f}},
            {{ 0.2f, -0.2f,  0.2f}, {0.0f, -1.0f, 0.0f}},
            {{-0.2f, -0.2f, -0.2f}, {0.0f, -1.0f, 0.0f}},
            {{ 0.2f, -0.2f,  0.2f}, {0.0f, -1.0f, 0.0f}},
            {{-0.2f, -0.2f,  0.2f}, {0.0f, -1.0f, 0.0f}},
        }
    {

        m_indices = {
            0, 1, 2, 3, 4, 5,       // Front face
            6, 7, 8, 9, 10, 11,     // Back face
            12, 13, 14, 15, 16, 17, // Left face
            18, 19, 20, 21, 22, 23, // Right face
            24, 25, 26, 27, 28, 29, // Top face
            30, 31, 32, 33, 34, 35  // Bottom face
        };


    }


private:
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
};


#endif // MESH_H
