#version 330 core

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec3 vNormal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 Normal;
out vec3 FragPos;

void main() {
    
    Normal = mat3(transpose(inverse(modelMatrix))) * vNormal;

    FragPos = vec3(modelMatrix * vPosition);

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vPosition;
}