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
uniform PointLight pointLights[8];

// Posición del observador/cámara
uniform vec3 viewPos;

// Número de luces activas
uniform int numLights;

void main() {
vec3 norm = normalize(Normal);
vec3 viewDir = normalize(viewPos - FragPos);
vec3 result = vec3(0.0);

for (int i = 0; i < numLights; ++i) {
// Vector hacia la luz
vec3 lightDir = normalize(pointLights[i].position - FragPos);

// Componente ambiente (reducida para evitar saturación)
vec3 ambient = pointLights[i].ambient * material.ambient * 0.3;

// Componente difusa
float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = pointLights[i].diffuse * (diff * material.diffuse) * 0.7;

// Componente especular (Blinn-Phong)
vec3 halfwayDir = normalize(lightDir + viewDir);
float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
vec3 specular = pointLights[i].specular * (spec * material.specular) * 0.5;

// Atenuación con valores mínimos seguros
float distance = length(pointLights[i].position - FragPos);
float a = max(pointLights[i].a, 1.0); // Asegura un mínimo de 1.0
float b = max(pointLights[i].b, 0.01); // Asegura un mínimo de 0.01
float c = max(pointLights[i].c, 0.0); // Puede ser 0
float attenuation = 1.0 / (a + b * distance + c * (distance * distance));

// Suma la contribución de esta luz
result += (ambient + diffuse + specular) * attenuation;
}

// Limitar el color final para evitar saturación
fragColor = vec4(clamp(result, 0.0, 1.0), 1.0);
}