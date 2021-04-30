//
// Created by Andrew on 3/10/2021.
//

#include "mesh.h"

#include <glad/glad.h>

Mesh::Mesh(vector<Vertex> &vertices, vector<unsigned int> &indices, vector<Texture> &textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    setup_mesh();
}

void Mesh::setup_mesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);

    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // Vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // Vertex texture coordinates
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_coordinates));

    glBindVertexArray(0);
}

void Mesh::draw(const Shader *shader) {
    unsigned int diffuse_idx = 1;
    unsigned int specular_idx = 1;
    unsigned int texture_idx = 0;

    // Normal textures
    if (textures.empty()) {
        shader->setBool("use_texture", false);
    } else {
        shader->setBool("use_texture", true);
    }

    for (unsigned int i = 0; i < textures.size(); ++i, ++texture_idx) {
        glActiveTexture(GL_TEXTURE0 + texture_idx); // Activate proper texture unit before binding

        // Retrieve texture number (the N in diffuse_textureN)
        string number;
        string name = textures[i].type;
        if(name == "texture_diffuse")
            number = std::to_string(diffuse_idx++);
        else if(name == "texture_specular")
            number = std::to_string(specular_idx++);

//        shader->setInt("material." + name + number, static_cast<int>(i));
        shader->setInt(name + number, static_cast<int>(texture_idx));
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    // Draw call
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    // Unbind
    glBindVertexArray(0);
    for (unsigned int i = 0; i < texture_idx; i++) {
        glActiveTexture(GL_TEXTURE0 + i); // Activate proper texture unit before binding
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Mesh::draw_depth() {
    // Draw call
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    // Unbind
    glBindVertexArray(0);
}