#version 430

out vec4 fragColor;

// Input du geometry shader
in vec2 UV;

uniform vec4 color = vec4(1.0, 0.0, 0.0, 1.0);

uniform sampler2D smokeTex;

void main(void)
{
    // Récupérer la couleur de la texture
    vec4 texColor = texture(smokeTex, UV);
    
    // Éliminer les fragments transparents
    if (texColor.a < 0.1) {
        discard;
    }
    
    // Couleur finale du fragment
    fragColor = texColor;
}