#ifndef EMPTYGL_SCENE_H
#define EMPTYGL_SCENE_H

#include <Eigen/Dense>

#include <vector>
#include <string>

#include <assimp/scene.h>

#include "shader.h"
#include "mesh.h"

using std::vector;
using std::string;

class Scene {
public:
    Scene(const vector<string> &path_list);
    Scene(const vector<string> &path, bool with_texture);
    void draw(const Shader *shader);
    void draw_depth();
private:
    // model data
    vector<Mesh> meshes;
    string directory;
    vector<Mesh::Texture> textures_loaded;

    void loadModel(const string &path, bool with_texture=true);
    void processNode(aiNode *node, const aiScene *scene, bool with_texture=true);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene, bool with_texture=true);
    vector<Mesh::Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const string &typeName);
    static unsigned int generateTextureFromFile(const char *path, const string &directory);
};

#endif //EMPTYGL_SCENE_H

