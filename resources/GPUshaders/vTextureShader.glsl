#version 330 core

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vCoordTexture;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;

void main() {
    // Calcular posición final del vértice
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vPosition;
    
    // Transformar normal a coordenadas de mundo
    Normal = mat3(transpose(inverse(modelMatrix))) * vNormal;
    
    // Calcular posición en coordenadas de mundo
    FragPos = vec3(modelMatrix * vPosition);
    
    // Pasar coordenadas de textura al fragment shader
    TexCoord = vCoordTexture;
}