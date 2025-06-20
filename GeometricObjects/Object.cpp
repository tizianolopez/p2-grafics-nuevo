#include "Object.hpp"

Object::Object()
{
    material = nullptr;
    textureID = 0;
    
}

void Object::setMaterial(shared_ptr<GPUMaterial> m) {
    material = m;
    textureID = 0;
    
    // Matriz para transformar el model es obligatorio porque cada objeto puede tener su propia transformación
    modelMatrix = glm::mat4(1.0f);


}

shared_ptr<GPUMaterial> Object::getMaterial() {
    return material;
}

void Object::materiaToGPU() {
    material->toGPU(program);
}


// Ya NO necesitos el buffer de colores (color_buffer y objectColors), porque el color del objeto lo dará el material y no un color por vértice. Por eso, eliminamos todo lo relacionado con color_buffer y objectColors.
void Object::toGPU(GLuint p)
{
    this->program = p;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1, &normal_buffer);

    glBindVertexArray(VAO);

    // --- VÉRTICES ---
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * objectVertices.size(), &objectVertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0); // layout location 0
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

    // --- NORMALES ---
    // Si tus normales son vec4, conviértelas a vec3 para el buffer
    std::vector<glm::vec3> normalsFiltered;
    normalsFiltered.reserve(objectNormals.size());
    for (const auto& n : objectNormals)
        normalsFiltered.push_back(vec3(n));

    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * normalsFiltered.size(), &normalsFiltered[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1); // layout location 1
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // --- MATERIAL ---
    // PASO 3: Antes de dibujar el objeto, enviamos su material a la GPU.
    // Así, el shader podrá acceder a los valores del material (Ka, Kd, Ks, shininess)
    // y calcular el color del objeto correctamente.
    if (material) material->toGPU(program);
}

void Object::draw(){
    // Antes de dibujar me aseguro de pasar modematrix al shader, asi cada objeto puede tener su propia transformacion
    GLuint modelMatrixLoc = glGetUniformLocation(program, "modelMatrix");
    
    if(modelMatrixLoc!=-1){
        glUniformMatrix4fv(modelMatrixLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    }

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, objectVertices.size());
    glBindVertexArray(0);
}




// Mètodes per tractar les textures

// Metode per inicialitzar la textura un cop llegida de disc
void Object::initTextureGL(const char *filename)
{
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // flip y-axis during image loading
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
    
    if (data) {
        if (nrChannels == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        } else if (nrChannels == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << "Texture loaded successfully: " << filename << ", channels: " << nrChannels << std::endl;
    } else {
        std::cerr << "Failed to load texture: " << filename << std::endl;
    }
    
    stbi_image_free(data);

    // Optionally store the texture ID in your cub object for later use
    
}


// Reactivar la textura en cas de canvi de shader al shader de textura
void Object::rebindTexture() 
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Set texture sampler uniform
    GLuint textureLoc = glGetUniformLocation(program, "textureMap");
    if (textureLoc != -1) {
        glUniform1i(textureLoc, 0);
    }
}

// Mètode per passar els vertexs de textura a la GPU
void Object::toGPUTexture(GLuint p)
{
    this->program = p;
    
    glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &vertex_buffer);
	glGenBuffers(1, &color_buffer);
    glGenBuffers(1, &texture_buffer);

    glBindVertexArray(VAO);
	// Bind vertices to layout location 0
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer );
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * objectVertices.size(), &objectVertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); // This allows usage of layout location 0 in the vertex shader
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

	// Bind colors to layout location 1
	glBindBuffer(GL_ARRAY_BUFFER, color_buffer );
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec4) * objectColors.size(), &objectColors[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1); // This allows usage of layout location 1 in the vertex shader
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

    // Bind texture coordinates to layout location 2
    glBindBuffer(GL_ARRAY_BUFFER, texture_buffer );
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * objectTextures.size(), &objectTextures[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2); // This allows usage of layout location 2 in the vertex shader
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

    // TO DO: A modificar per a considerar les normals

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Object::setmodelMatrix(glm::mat4 m){
    modelMatrix = m;
}