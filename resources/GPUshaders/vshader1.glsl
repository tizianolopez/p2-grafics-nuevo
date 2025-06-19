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

uniform PointLight pointLight;

out vec4 color;



void main() {
    // Calculate world space position
    gl_Position = projectionMatrix*viewMatrix*modelMatrix * vPosition;

    // Para comprobar la luz 0 (roja)
    // color = vec4(pointLights[0].diffuse, 1.0);

    // Para comprobar la luz 1 (verde)
    // color = vec4(pointLights[1].diffuse, 1.0);

    // Para comprobar la luz 2 (azul)
    color = vec4(pointLights[2].diffuse, 1.0);
}