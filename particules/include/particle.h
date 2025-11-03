#ifndef PARTICLE_H
#define PARTICLE_H
#include <QVector3D>
#include <cmath>

// Structure alignée pour correspondre au layout std430 du compute shader
struct Particle {
    QVector3D pos;      // 12 bytes + 4 bytes padding = 16 bytes
    float age;          // 4 bytes
    QVector3D speed;    // 12 bytes + 4 bytes padding = 16 bytes  
    float ageMax;       // 4 bytes
    // Total: 40 bytes (aligné sur std430)

    void init() {
        pos = QVector3D(0.0, 0.0, 0.0);
        float angle = 2.0 * M_PI * rand() / RAND_MAX;
        // Augmenter la vitesse horizontale pour des directions plus variées
        float horizontalSpeed = 1.0f + 2.0f * rand() / static_cast<float>(RAND_MAX);
        // Vitesse verticale aléatoire
        float verticalSpeed = 3.0f + 5.0f * rand() / static_cast<float>(RAND_MAX);
        speed = QVector3D(horizontalSpeed * cos(angle), horizontalSpeed * sin(angle), verticalSpeed);
        age = 0.0f;
        ageMax = 50.0f + (100.0f * rand() / float(RAND_MAX));
    }

    /*
    void animate(float deltaTime) {
        // Limiter deltaTime pour éviter des sauts trop importants
        if (deltaTime > 0.1f) deltaTime = 0.1f;
        
        // Constante de gravité (réduite pour un effet visuel plus agréable)
        const float gravity = 3.0f;
        
        // Application de la gravité sur l'axe Z (vers le bas)
        speed[2] -= gravity * deltaTime;
        
        // Mise à jour de la position avec deltaTime
        pos += speed * deltaTime;

        // Gestion de la collision avec le sol
        if (pos[2] < 0.0) {
            speed[2] = -0.8 * speed[2];
            pos[2] = 0.0;
        }
        
        // Mise à jour de l'âge avec deltaTime
        age += deltaTime;
        if(age >= ageMax) init();
    }
    */
};

#endif // PARTICLE_H
