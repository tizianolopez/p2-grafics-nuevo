#pragma once

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GPULight.hpp"
#include "GPUPointLight.hpp"


class GPULightsManager  {
    private:
        // Vector de llums contingudes la mon
        vector<shared_ptr<GPUPointLight>> lights;
        // Llum ambient global:
        vec3  ambientLight;

    public:
        GPULightsManager()  {
            ambientLight = vec3(0.2f, 0.2f, 0.2f);
        }
        
        void setAmbientLight(vec3 a) {
            ambientLight = a;
        }

        void toGPU(GLuint program) {
        // Si el identificador del programa es 0, no hacemos nada
        if (program == 0) return;

        // 1. Enviar la luz ambiental global a la GPU
        // Esto asegura que el shader tenga el valor correcto de ambientLight
        ambientLightToGPU(program, ambientLight);

        // 2. Enviar las luces puntuales activas a la GPU
        // Recorremos el vector de luces y enviamos solo las que están activas
        int numActiveLights = 0;
        for (size_t i = 0; i < lights.size(); i++) {
            if (lights[i]->isEnabled()) {
                lights[i]->toGPU(program); // Envía los datos de la luz puntual al shader
                numActiveLights++;         // Contamos cuántas luces activas hay
            }
        }

        // 3. Enviar al shader el número de luces activas (si el shader lo necesita)
        GLuint numLightsLocation = glGetUniformLocation(program, "numLights");
        if (numLightsLocation != static_cast<GLuint>(-1)) {
            glUniform1i(numLightsLocation, numActiveLights);
        }
        // Si el uniform no existe, no pasa nada (puedes añadir un mensaje de depuración si quieres)
    }
        

        void addLight(shared_ptr<GPUPointLight> l) {
            lights.push_back(l);
        }

        void ambientLightToGPU(GLuint program, vec3 a) {
            ambientLight = a;
            // TO DO: Enviar la llum ambient global a la GPU i revisar des d'on es crida aquest mètode 
            GLuint ambientLightLoc = glGetUniformLocation(program, "ambientLight");
            if (ambientLightLoc != -1) {
                glUniform3fv(ambientLightLoc, 1, glm::value_ptr(ambientLight));
            }
            
        }

        void updateAllLights(GLuint program, vector<GPUPointLight> &lightsNew) {
            if (program == 0) return;
            
            lights.clear();

            for (int i = 0; i < lightsNew.size(); i++) {
                lights.push_back(make_shared<GPUPointLight>(lightsNew[i].getPos(), lightsNew[i].getIa(), lightsNew[i].getId(), lightsNew[i].getIs(), 0.0f, 0.0f, 1.0f, lightsNew[i].isEnabled()));
            }

            // TO DO Fitxa 2: Cal enviar totes les llums a la GPU des d'aquest mètode?
            
        }

        void updateSingleLight(GLuint program, GPUPointLight &pl, int index) {
            if (program == 0) return;
            
            lights[index] = make_shared<GPUPointLight>(pl.getPos(), pl.getIa(), pl.getId(), pl.getIs(), 0.0f, 0.0f, 1.0f);
            
            // TO DO Fitxa 2: Cal només enviar la llum que està a "index" del vector de llums  a la GPU des d'aquest mètode?
            
        }


};