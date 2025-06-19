#include "GPUPointLight.hpp"

GPUPointLight::GPUPointLight(vec3 posicio, vec3 Ia, vec3 Id, vec3 Is): GPULight(Ia, Id, Is) {
    this->pos = posicio;
    this->a = 0.0f;
    this->b = 0.0f;
    this->c = 0.0f;
}

GPUPointLight::GPUPointLight(vec3 posicio, vec3 Ia, vec3 Id, vec3 Is, float a, float b, float c): GPULight(Ia, Id, Is) {
    this->pos = posicio;
    this->a = a;
    this->b = b;
    this->c = c;
}

GPUPointLight::GPUPointLight(vec3 posicio, vec3 Ia, vec3 Id, vec3 Is, float a, float b, float c, bool isEnabled): GPULight(Ia, Id, Is, isEnabled) {
    this->pos = posicio;
    this->a = a;
    this->b = b;
    this->c = c;
}

vec3 GPUPointLight::vectorL(vec3 point) {
    return normalize(pos - point);
}

float GPUPointLight::attenuation(vec3 point) {
    if (abs(a)<DBL_EPSILON && abs(b)<DBL_EPSILON  && abs(c)<DBL_EPSILON) {
        //Si tots els coeficients son 0 considerem que no hi ha atenuacio
        return 1.0f;
    }
    //Calculem la distancia entre el punt i la posicio de la llum
    float d = distance(point, pos);
    return 1.0f/(c*d*d + b*d + a);
}

float GPUPointLight::distanceToLight(vec3 point) {
    return distance(point, pos);
}

void GPUPointLight::toGPU(GLuint pr) {
    GPULight::toGPU(pr);    // Llama al método toGPU de la clase base GPULight. Esto normalmente envía los colores de la luz (ambient, diffuse, specular) si está implementado ahí.

    GLint posLoc = glGetUniformLocation(pr, "pointLight.position");     // PASO 1: Enviar la posición de la luz puntual al shader. Busca la localización del uniform 'pointLight.position' en el shader.

    if (posLoc != -1) glUniform3fv(posLoc, 1, glm::value_ptr(pos));    // Si existe, envía el valor de 'pos' (vec3) a la GPU.


    // PASO 2 (opcional): Envio los parámetros de atenuación si los uso en el shader.
    // Busco la localización de cada parámetro y los envío.
    GLint aLoc = glGetUniformLocation(pr, "pointLight.a"); //constant term
    GLint bLoc = glGetUniformLocation(pr, "pointLight.b"); //linear term
    GLint cLoc = glGetUniformLocation(pr, "pointLight.c"); //quadratic term
    if (aLoc != -1) glUniform1f(aLoc, a);
    if (bLoc != -1) glUniform1f(bLoc, b);
    if (cLoc != -1) glUniform1f(cLoc, c);

    // PASO 3: Enviar los colores de la luz (ambient, diffuse, specular) al shader.
    // Busca la localización de cada campo y los envía.
    GLint ambLoc = glGetUniformLocation(pr, "pointLight.ambient");
    GLint diffLoc = glGetUniformLocation(pr, "pointLight.diffuse");
    GLint specLoc = glGetUniformLocation(pr, "pointLight.specular");
    if (ambLoc != -1) glUniform3fv(ambLoc, 1, glm::value_ptr(getIa())); // Color ambiente
    if (diffLoc != -1) glUniform3fv(diffLoc, 1, glm::value_ptr(getId())); // Color difuso
    if (specLoc != -1) glUniform3fv(specLoc, 1, glm::value_ptr(getIs())); // Color especular
}

void GPUPointLight::updateToGPU(int index) {
    // TO DO: PAS 2.3: actualitzar les propietats de la llum puntual a la GPU
    // Cal obtenir els identificadors de les variables uniform de la GPU de la llum amb index "index"
    // i actualitzar els seus valors amb les propietats de la llum puntual

    // Enviar cada atributo de la luz al índice correspondiente del array en el shader
    std::string prefix = "pointLights[" + std::to_string(index) + "].";

    GLint posLoc = glGetUniformLocation(program, (prefix + "position").c_str());
    if (posLoc != -1) glUniform3fv(posLoc, 1, glm::value_ptr(pos));

    GLint aLoc = glGetUniformLocation(program, (prefix + "a").c_str());
    GLint bLoc = glGetUniformLocation(program, (prefix + "b").c_str());
    GLint cLoc = glGetUniformLocation(program, (prefix + "c").c_str());
    if (aLoc != -1) glUniform1f(aLoc, a);
    if (bLoc != -1) glUniform1f(bLoc, b);
    if (cLoc != -1) glUniform1f(cLoc, c);

    GLint ambLoc = glGetUniformLocation(program, (prefix + "ambient").c_str());
    GLint diffLoc = glGetUniformLocation(program, (prefix + "diffuse").c_str());
    GLint specLoc = glGetUniformLocation(program, (prefix + "specular").c_str());
    if (ambLoc != -1) glUniform3fv(ambLoc, 1, glm::value_ptr(getIa()));
    if (diffLoc != -1) glUniform3fv(diffLoc, 1, glm::value_ptr(getId()));
    if (specLoc != -1) glUniform3fv(specLoc, 1, glm::value_ptr(getIs()));

}
