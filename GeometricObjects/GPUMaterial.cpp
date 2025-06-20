#include "GPUMaterial.hpp"


GPUMaterial::GPUMaterial(): Ka(0.2f), Kd(1.0f), Ks(0.8f) {
    shininess = 100.0f;
    opacity = 1;
};
GPUMaterial::GPUMaterial(vec3 d):  Ka(0.2f), Kd(d), Ks(0.8f) {
    shininess = 100.0f;
    opacity = 1;
};

GPUMaterial::GPUMaterial(vec3 a, vec3 d, vec3 s, float shininess):  Ka(a), Kd(d), Ks(s), shininess(shininess) {
    opacity = 1;
};

void GPUMaterial::toGPU(GLuint program) {
     // Set material properties to GPU
    // TO DO: PAS 3.1: Enviar les propietats del material a la GPU
    void GPUMaterial::toGPU(GLuint program) {
    // Enviamos los atributos del material como uniform struct
    GLint kaLoc = glGetUniformLocation(program, "material.ambient");
    GLint kdLoc = glGetUniformLocation(program, "material.diffuse");
    GLint ksLoc = glGetUniformLocation(program, "material.specular");
    GLint shLoc = glGetUniformLocation(program, "material.shininess");

    if (kaLoc != -1) glUniform3fv(kaLoc, 1, glm::value_ptr(Ka));
    if (kdLoc != -1) glUniform3fv(kdLoc, 1, glm::value_ptr(Kd));
    if (ksLoc != -1) glUniform3fv(ksLoc, 1, glm::value_ptr(Ks));
    if (shLoc != -1) glUniform1f(shLoc, shininess);
}

}