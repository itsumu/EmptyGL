//
// Created by Andrew on 3/10/2021.
//

#ifndef TOONSHADING_MESH_H
#define TOONSHADING_MESH_H

#include <string>
#include <vector>

#include <Eigen/Dense>

#include "shader.h"

using std::string;
using std::vector;

class Mesh {
public:
    struct Vertex {
        Eigen::Vector3f position;
        Eigen::Vector3f normal;
        Eigen::Vector2f texture_coordinates;
    };
    struct Texture {
        unsigned int id;
        string type;
        string path;
    };

    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    Mesh(vector<Vertex> &vertices, vector<unsigned int> &indices, vector<Texture> &textures);
    void draw(const Shader *shader);
    void draw_depth();
private:
    unsigned int VAO, VBO, EBO;

    void setup_mesh();
};


#endif //TOONSHADING_MESH_H
