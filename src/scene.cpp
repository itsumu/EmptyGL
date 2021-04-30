#include "scene.h"

#include <iostream>

#include <Eigen/Dense>
#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using std::cout;
using std::endl;
typedef Mesh::Texture Texture;
typedef Mesh::Vertex Vertex;

Scene::Scene(const vector<string> &path_list) {
    for (auto &path: path_list) {
        loadModel(path);
    }
}

Scene::Scene(const vector<string> &path_list, bool with_texture) {
    for (auto &path: path_list) {
        loadModel(path, with_texture);
    }
}

void Scene::draw(const Shader *shader) {
    for (auto &mesh: meshes)
        mesh.draw(shader);
}

void Scene::draw_depth() {
    for (auto &mesh: meshes)
        mesh.draw_depth();
}

void Scene::loadModel(const string &path, bool with_texture) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene, with_texture);
}

void Scene::processNode(aiNode *node, const aiScene *scene, bool with_texture) {
    // process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene, with_texture));
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene, with_texture);
    }
}

Mesh Scene::processMesh(aiMesh *mesh, const aiScene *scene, bool with_texture) {
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    // Process vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        // Process vertex positions, normals and texture coordinates
        vertex.position[0] = mesh->mVertices[i].x;
        vertex.position[1] = mesh->mVertices[i].y;
        vertex.position[2] = mesh->mVertices[i].z;
        vertex.normal[0] = mesh->mNormals[i].x;
        vertex.normal[1] = mesh->mNormals[i].y;
        vertex.normal[2] = mesh->mNormals[i].z;
        if (mesh->mTextureCoords[0]) { // Check existence of texture coordinates
            Eigen::Vector2f vec;
            vec[0] = mesh->mTextureCoords[0][i].x;
            vec[1] = mesh->mTextureCoords[0][i].y;
            vertex.texture_coordinates = vec;
        } else {
            vertex.texture_coordinates = Eigen::Vector2f(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }
    // Process indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    // Process material
    if(mesh->mMaterialIndex >= 0 && with_texture) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        vector<Texture> diffuseMaps = loadMaterialTextures(material,
                                                           aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        vector<Texture> specularMaps = loadMaterialTextures(material,
                                                            aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

vector<Texture> Scene::loadMaterialTextures(aiMaterial *mat, aiTextureType type, const string &typeName) {
    vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        bool skip = false;

        mat->GetTexture(type, i, &str);
        // Check texture whether loaded before
        for (unsigned int j = 0; j < textures_loaded.size(); j++) {
            if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) { // Texture has been loaded before
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if(!skip) { // Texture hasn't been loaded already, load it
            Texture texture;
            texture.id = generateTextureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture); // add to loaded textures
        }
    }
    return textures;
}

unsigned int Scene::generateTextureFromFile(const char *path, const string &directory) {
    string filename = string(path);
    filename = directory + '/' + filename;

    // Generate texture object
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, n_channels;
    unsigned char *texture_data = stbi_load(filename.c_str(), &width, &height, &n_channels, 0);
    if (texture_data) {
        GLenum format;
        if (n_channels == 1)
            format = GL_RED;
        else if (n_channels == 3)
            format = GL_RGB;
        else if (n_channels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, texture_data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(texture_data);
    }
    else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(texture_data);
    }

    return textureID;
}
