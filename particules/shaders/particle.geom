#version 430

#extension GL_EXT_geometry_shader4 : enable
#extension GL_EXT_gpu_shader4 : enable

layout ( points ) in;

// Shader de passage : passe de point vers point
//layout ( points ) out;
// Shader utile : passe de point vers triangles

layout ( triangle_strip, max_vertices = 4 ) out;

// Les matrices peuvent être réutilisées
uniform mat4 mvp;

// Variables d'entrées : des tableaux
in vec3 initialVertPos[];

// Variables de sorties : des éléments uniques
out vec2 UV;

//Geometry Shader entry point
void main(void) {
	// Taille d'une particule
	float scale = 0.05;

	// Utiliser la position NON transformée pour créer le quad
    vec3 particlePos = initialVertPos[0];
    
    // Créer un quad (billboard) centré sur la particule
    // Coin inférieur gauche
    gl_Position = mvp * vec4(particlePos + vec3(-scale, -scale, 0.0), 1.0);
    UV = vec2(0.0, 0.0);
    EmitVertex();
    
    // Coin inférieur droit
    gl_Position = mvp * vec4(particlePos + vec3(scale, -scale, 0.0), 1.0);
    UV = vec2(1.0, 0.0);
    EmitVertex();
    
    // Coin supérieur gauche
    gl_Position = mvp * vec4(particlePos + vec3(-scale, scale, 0.0), 1.0);
    UV = vec2(0.0, 1.0);
    EmitVertex();
    
    // Coin supérieur droit
    gl_Position = mvp * vec4(particlePos + vec3(scale, scale, 0.0), 1.0);
    UV = vec2(1.0, 1.0);
    EmitVertex();
    
	EndPrimitive();
}