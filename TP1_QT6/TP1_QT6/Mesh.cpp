#include "Mesh.h"
#include "Vec3.h"

#include <QOpenGLFunctions>


void Mesh::createRectangleMesh(float length, float width, float height, const Vec3<float>& center)
{
    m_vertices.clear();
    m_indices.clear();

    Vec3<float> color = {1.0f,1.0f,1.0f};

    // Décalage pour centrer le cube
    float hx = length  * 0.5f;
    float hy = width   * 0.5f;
    float hz = height  * 0.5f;

    // 8 coins autour du center
    Vec3<float> p000 = {center.x - hx, center.y - hy, center.z - hz};
    Vec3<float> p100 = {center.x + hx, center.y - hy, center.z - hz};
    Vec3<float> p010 = {center.x - hx, center.y + hy, center.z - hz};
    Vec3<float> p110 = {center.x + hx, center.y + hy, center.z - hz};
    Vec3<float> p001 = {center.x - hx, center.y - hy, center.z + hz};
    Vec3<float> p101 = {center.x + hx, center.y - hy, center.z + hz};
    Vec3<float> p011 = {center.x - hx, center.y + hy, center.z + hz};
    Vec3<float> p111 = {center.x + hx, center.y + hy, center.z + hz};

    // Ajouter les 8 sommets uniques (normales à calculer ou trivial)
    m_vertices.push_back({p000, {}, color}); // 0
    m_vertices.push_back({p100, {}, color}); // 1
    m_vertices.push_back({p010, {}, color}); // 2
    m_vertices.push_back({p110, {}, color}); // 3
    m_vertices.push_back({p001, {}, color}); // 4
    m_vertices.push_back({p101, {}, color}); // 5
    m_vertices.push_back({p011, {}, color}); // 6
    m_vertices.push_back({p111, {}, color}); // 7


    // Faces (indices)
    m_indices.insert(m_indices.end(), {4,5,7, 4,7,6}); // avant Z+
    m_indices.insert(m_indices.end(), {0,3,1, 0,2,3}); // arrière Z-
    m_indices.insert(m_indices.end(), {0,6,2, 0,4,6}); // gauche X-
    m_indices.insert(m_indices.end(), {1,3,7, 1,7,5}); // droite X+
    m_indices.insert(m_indices.end(), {2,6,7, 2,7,3}); // haut Y+
    m_indices.insert(m_indices.end(), {0,1,5, 0,5,4}); // bas Y-

    computeNormals();
}


void Mesh::setupBuffers() {
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    if (!m_buffersCreated) {
        m_vao.create();
        m_vbo = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
        m_vbo.create();
        m_ebo = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
        m_ebo.create();
        m_buffersCreated = true;
    }

    m_vao.bind();

    m_vbo.bind();
    m_vbo.allocate(m_vertices.data(), m_vertices.size() * sizeof(Vertices));

    m_ebo.bind();
    m_ebo.allocate(m_indices.data(), m_indices.size() * sizeof(unsigned int));

    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertices), reinterpret_cast<void*>(0));

    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertices), reinterpret_cast<void*>(sizeof(Vec3<float>)));

    f->glEnableVertexAttribArray(2);
    f->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertices), reinterpret_cast<void*>(sizeof(Vec3<float>)*2));

    m_vao.release();
    m_vbo.release();
    m_ebo.release();
}


void Mesh::draw(){
    m_vao.bind();
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, reinterpret_cast<void*>(0));
    m_vao.release();
}




void Mesh::computeNormals() {
    for (size_t i = 0; i < m_indices.size(); i += 3) {
        unsigned int i0 = m_indices[i];
        unsigned int i1 = m_indices[i + 1];
        unsigned int i2 = m_indices[i + 2];

        Vec3<float> p0 = m_vertices[i0].position;
        Vec3<float> p1 = m_vertices[i1].position;
        Vec3<float> p2 = m_vertices[i2].position;

        Vec3<float> edge1 = p1 - p0;
        Vec3<float> edge2 = p2 - p0;

        Vec3<float> normal = edge1.cross(edge2);

        m_vertices[i0].normal += normal;
        m_vertices[i1].normal += normal;
        m_vertices[i2].normal += normal;
    }

    for (auto& v : m_vertices) {
        v.normal.normalize();
    }
}

void Mesh::destroyBuffers() {
    if (!m_buffersCreated) return;
    m_vao.destroy();
    m_vbo.destroy();
    m_ebo.destroy();
    m_buffersCreated = false;
}



