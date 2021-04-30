//
// Created by andrew on 9/15/20.
//

#include "shader.h"

#include <iostream>
#include <fstream>

#include <glad/glad.h>

using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;

unsigned int Shader::generateVertexShader(const std::string &vertex_shader_source) {
    int success;
    char info_log[512];
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    const char * vertex_shader_source_char = vertex_shader_source.c_str();

    glShaderSource(vertex_shader, 1, &vertex_shader_source_char, nullptr);
    glCompileShader(vertex_shader);

    // Compilation check
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << endl;
    }
    return vertex_shader;
}

unsigned int Shader::generateFragmentShader(const std::string &fragment_shader_source) {
    int success;
    char info_log[512];
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    const char * fragment_shader_source_char = fragment_shader_source.c_str();

    glShaderSource(fragment_shader, 1, &fragment_shader_source_char, nullptr);
    glCompileShader(fragment_shader);

    // Compilation check
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << endl;
    }
    return fragment_shader;
}

unsigned int Shader::linkShaders(unsigned int vertex_shader, unsigned int fragment_shader) {
    int success;
    char info_log[512];
    unsigned int shader_program = glCreateProgram();

    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program); // Link

    // Linking check
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << endl;
    }

    // Delete individual shaders
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    return shader_program;
}

Shader::Shader(const std::string &vertex_path, const std::string &fragment_path) {
    // Generate & Compile vertex shader + fragment shader
    ifstream vertex_shader_stream(vertex_path);
    if (!vertex_shader_stream.is_open()) {
        cerr << "Failed to open file: " << vertex_path << endl;
    }
    std::string vertex_shader_string((std::istreambuf_iterator<char>(vertex_shader_stream)),
                                      std::istreambuf_iterator<char>());
    const char *vertex_shader_source = vertex_shader_string.c_str();
    vertex_shader_stream.close();
    unsigned int vertex_shader = generateVertexShader(vertex_shader_source);

    ifstream fragment_shader_stream(fragment_path);
    if (!fragment_shader_stream.is_open()) {
        cerr << "Failed to open file: " << fragment_path << endl;
    }
    std::string fragment_shader_string((std::istreambuf_iterator<char>(fragment_shader_stream)),
                                  std::istreambuf_iterator<char>());
    const char *fragment_shader_source = fragment_shader_string.c_str();
    vertex_shader_stream.close();
    unsigned int fragment_shader = generateFragmentShader(fragment_shader_source);

    // Link shader programs
    ID = linkShaders(vertex_shader, fragment_shader);
}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::release() {
    glDeleteProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::set4f(const std::string &name, float value[]) const {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), value[0], value[1], value[2], value[3]);
}

void Shader::setMat4(const std::string &name, const Eigen::Matrix4f &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, mat.data());
}