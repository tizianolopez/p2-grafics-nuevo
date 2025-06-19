#version 330 core

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 vColor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec3 ambientLight;

out vec4 color;



void main() {
    // Calculate world space position
    gl_Position = projectionMatrix*viewMatrix*modelMatrix * vPosition;
    color = vec4(ambientLight, 1.0);
}