//
// Created by andrew on 9/15/20.
//

#ifndef EMPTYGL_SHADER_H
#define EMPTYGL_SHADER_H

#include <string>

#include <Eigen/Dense>

class Shader {
private:
    unsigned int generateVertexShader(const std::string &vertex_shader_source);
    unsigned int generateFragmentShader(const std::string &fragment_shader_source);
    unsigned int linkShaders(unsigned int vertex_shader, unsigned int fragment_shader);

public:
    // Program ID
    unsigned int ID;

    // Constructor reads and builds the shader
    Shader(const std::string &vertex_path, const std::string &fragment_path);
    // Use/Activate the shader
    void use();
    // Release program
    void release();
    // Utility uniform functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void set4f(const std::string &name, float value[]) const;
    void setMat4(const std::string &name, const Eigen::Matrix4f &mat) const;
};

#endif //EMPTYGL_SHADER_H
