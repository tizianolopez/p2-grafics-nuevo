#include "Lights/GPULight.hpp"

//toGPU(GLuint program) normalmente envía los datos de UNA luz a la GPU, usando nombres de uniforms como "pointLight.diffuse". Se usa cuando solo tenemos UNA luz activa o queremos enviar los datos de una luz específica
void GPULight::toGPU(GLuint p) {
    // TO DO: enviar les propietats de Ia, Id i Is a la GPU. Pas 2.2
    program = p;

}


//updateToGPU(int index) envía los datos de UNA luz pero al índice concreto del array de luces en el shader. Es útil si solo ha cambiado una luz, así no tienes que reenviar los datos de todas.
void GPULight::updateToGPU(int index) {
    // TO DO: actualitzar les propietats de la llum a la GPU. Pas 2.3
    // Cal obtenir els identificadors de les variables uniform de la GPU
    // i actualitzar els seus valors amb les propietats de la llum

    // ----------------------
    // EJEMPLO DE LA FICHA (NO AFECTA AL FUNCIONAMIENTO ACTUAL)
    // Así es como propone la ficha gestionar los identificadores de los uniforms para arrays de structs en el shader.
    // Es más eficiente para proyectos grandes, pero para prácticas pequeñas es suficiente con la versión directa.
    // ----------------------

    // 1. Definir un struct para guardar las localizaciones de los uniforms de cada atributo de la luz
    /*
    struct gl_IdPointLight {
        GLuint pos;
        GLuint ambient;
        GLuint diffuse;
        GLuint specular;
        GLuint a, b, c;
        // ...otros atributos si los tienes
    };
    */

    // 2. Crear un array de estos structs, uno por cada luz (por ejemplo, MAX_LIGHTS = 8)
    /*
    gl_IdPointLight gl_IdPointLights[MAX_LIGHTS];
    */

    // 3. Al iniciar el programa (o al cargar el shader), obtienes y guardas todas las localizaciones:
    /*
    for (int i = 0; i < MAX_LIGHTS; ++i) {
        std::string prefix = "pointLights[" + std::to_string(i) + "].";
        gl_IdPointLights[i].pos      = glGetUniformLocation(program, (prefix + "position").c_str());
        gl_IdPointLights[i].ambient  = glGetUniformLocation(program, (prefix + "ambient").c_str());
        gl_IdPointLights[i].diffuse  = glGetUniformLocation(program, (prefix + "diffuse").c_str());
        gl_IdPointLights[i].specular = glGetUniformLocation(program, (prefix + "specular").c_str());
        gl_IdPointLights[i].a        = glGetUniformLocation(program, (prefix + "a").c_str());
        gl_IdPointLights[i].b        = glGetUniformLocation(program, (prefix + "b").c_str());
        gl_IdPointLights[i].c        = glGetUniformLocation(program, (prefix + "c").c_str());
        // ...otros atributos
    }
    */

    // 4. Cuando quieras actualizar los valores, solo usas glUniform* con los IDs guardados:
    /*
    for (int i = 0; i < numActiveLights; ++i) {
        if (gl_IdPointLights[i].pos != -1)
            glUniform3fv(gl_IdPointLights[i].pos, 1, glm::value_ptr(lights[i].getPos()));
        if (gl_IdPointLights[i].ambient != -1)
            glUniform3fv(gl_IdPointLights[i].ambient, 1, glm::value_ptr(lights[i].getIa()));
        if (gl_IdPointLights[i].diffuse != -1)
            glUniform3fv(gl_IdPointLights[i].diffuse, 1, glm::value_ptr(lights[i].getId()));
        // ...y así con todos los atributos
    }
    */

    // ----------------------
    // NOTA IMPORTANTE:
    // Este método es el que propone la ficha (enunciado) porque es más eficiente para muchas luces.
    // Sin embargo, para prácticas pequeñas y para aprender, es más sencillo y suficiente llamar a
    // glGetUniformLocation justo antes de cada glUniform*, como hacemos normalmente.
    // Así el código es más corto y fácil de mantener en proyectos pequeños.
    // Si implemento este sistema, deberías poner el struct y el array en GPULightsManager
    // y usarlo desde los métodos de GPUPointLight y GPULightsManager.
    // ----------------------
    
    
}
