#include "systemwindow.h"
#include <QMatrix4x4>
#include <QOpenGLShader>
#include <QScreen>
#include <QtMath>
#include <QDebug>
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_3_Core>
#include <QDateTime>
#include <QImage>

void ParticleSystemWindow::initialize() {
    m_context = new QOpenGLContext(this);
    m_context->create();

    //Switch to OpenGL context
    m_context->makeCurrent(this);
    gl43 = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_4_3_Core>();
    if (!gl43) {
        qFatal("Impossible to initialize OpenGLFunction 4.3 Core.");
    }
    gl43->initializeOpenGLFunctions();
    srand(static_cast<unsigned int>(QDateTime::currentMSecsSinceEpoch() & 0xFFFFFFFF));

    // Initialiser les particules
    particles.resize(numParticles);
    for (auto& p : particles) p.init();

    // Créer le SSBO (Shader Storage Buffer Object)
    gl43->glGenBuffers(1, &ssbo);
    gl43->glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    gl43->glBufferData(GL_SHADER_STORAGE_BUFFER, 
                       numParticles * sizeof(Particle), 
                       particles.data(), 
                       GL_DYNAMIC_DRAW);
    gl43->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
    gl43->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    // Créer le VBO pour le rendu
    vbo.create();

    m_camera.setAspectRatio(width() / float(height()));

    // Charger le compute shader
    computeProgram = new QOpenGLShaderProgram(this);
    if (!computeProgram->addShaderFromSourceFile(QOpenGLShader::Compute, "../shaders/particle.comp")) {
        qWarning() << "Erreur lors du chargement du compute shader:" << computeProgram->log();
    }
    if (!computeProgram->link()) {
        qWarning() << "Erreur lors du link du compute shader:" << computeProgram->log();
    }

    // Charger les shaders de rendu
    program = new QOpenGLShaderProgram(this);
    program->addShaderFromSourceFile(QOpenGLShader::Vertex, "../shaders/particle.vert");
    program->addShaderFromSourceFile(QOpenGLShader::Geometry, "../shaders/particle.geom");
    program->addShaderFromSourceFile(QOpenGLShader::Fragment, "../shaders/particle.frag");
    program->link();
    program->bind();

    matrixUniform = program->uniformLocation("mvp");

    // Charger la texture smoke.png
    QImage smokeImage("../data/smoke.png");
    if (smokeImage.isNull()) {
        qWarning() << "Impossible de charger la texture smoke.png";
    } else {
        smokeTexture = new QOpenGLTexture(smokeImage.mirrored());
        smokeTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        smokeTexture->setMagnificationFilter(QOpenGLTexture::Linear);
        smokeTexture->setWrapMode(QOpenGLTexture::ClampToEdge);
    }

    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(5.0f);
    
    // Activer le blending pour la transparence
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Initialiser le temps
    time.start();
    lastTime = 0;
}

void ParticleSystemWindow::render() {
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.f, 1.0f, 1.0f, 1.0f);

    // Calcul du deltaTime
    qint64 currentTime = time.elapsed(); // Temps en millisecondes
    float deltaTime = (currentTime - lastTime) / 1000.0f; // Conversion en secondes
    lastTime = currentTime;

    // ========== COMPUTE SHADER: Animation des particules sur GPU ==========
    computeProgram->bind();
    
    // Passer les uniforms au compute shader
    computeProgram->setUniformValue("deltaTime", deltaTime);
    computeProgram->setUniformValue("time", currentTime / 1000.0f);
    
    // Lier le SSBO
    gl43->glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
    
    // Lancer le compute shader
    // On utilise (numParticles + 255) / 256 groupes de travail pour couvrir toutes les particules
    // car local_size_x = 256 dans le shader
    GLuint numWorkGroups = (numParticles + 255) / 256;
    gl43->glDispatchCompute(numWorkGroups, 1, 1);
    
    // Attendre que le compute shader ait terminé avant de rendre
    gl43->glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
    
    computeProgram->release();

    // ========== RENDU: Afficher les particules ==========
    // Mapper le SSBO pour extraire les positions
    gl43->glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    Particle* particlesGPU = (Particle*)gl43->glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);
    
    if (particlesGPU) {
        particlePositions.clear();
        particlePositions.reserve(numParticles * 3);
        
        for (int i = 0; i < numParticles; ++i) {
            particlePositions.push_back(particlesGPU[i].pos.x());
            particlePositions.push_back(particlesGPU[i].pos.y());
            particlePositions.push_back(particlesGPU[i].pos.z());
        }
        
        gl43->glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    }
    gl43->glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    // Mettre à jour le VBO avec les nouvelles positions
    vbo.bind();
    vbo.allocate(particlePositions.data(), particlePositions.size() * sizeof(GLfloat));
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    // Rendu des particules
    program->bind();
    
    QMatrix4x4 mvp = m_camera.projectionMatrix() * m_camera.viewMatrix();
    program->setUniformValue(matrixUniform, mvp);

    // Lier la texture smoke
    if (smokeTexture) {
        smokeTexture->bind(0);
        program->setUniformValue("smokeTex", 0);
    }

    glDrawArrays(GL_POINTS, 0, numParticles);

    program->release();
}


