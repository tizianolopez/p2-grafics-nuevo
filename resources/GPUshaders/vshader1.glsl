#version 330 core

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 vColor;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec3 ambientLight;

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float a;
    float b;
    float c;
};

uniform PointLight pointLights[8];

out vec4 color;



void main() {
    // Calculate world space position
    gl_Position = projectionMatrix*viewMatrix*modelMatrix * vPosition;

    color = vec4(pointLights[0].diffuse, 1.0);
}