#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 fragColor;

// Estructura de material
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

// Estructura de luz puntual
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

// Posición del observador/cámara
uniform vec3 viewPos;

void main() {
    // Normalizada
    vec3 norm = normalize(Normal);
    // Vector hacia la luz
    vec3 lightDir = normalize(pointLight.position - FragPos);
    // Componente ambiente
    vec3 ambient = pointLight.ambient * material.ambient;

    // Componente difusa
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = pointLight.diffuse * (diff * material.diffuse);

    // Componente especular (Blinn-Phong)
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    vec3 specular = pointLight.specular * (spec * material.specular);

    // Atenuación (opcional)
    float distance = length(pointLight.position - FragPos);
    float attenuation = 1.0 / (pointLight.a + pointLight.b * distance + pointLight.c * (distance * distance));

    // Color final
    vec3 result = (ambient + diffuse + specular) * attenuation;
    fragColor = vec4(result, 1.0);
}