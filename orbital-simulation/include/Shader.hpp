#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

const std::string ERROR_LINE = "\n -- --------------------------------------------------- -- ";

class Shader {
    public:
        unsigned int ID; // OpenGL Program ID

        // Constructor - reads ands builds the shader
        Shader(const char* vertexPath, const char* fragmentPath) {
            std::string vertexCode;
            std::string fragmentCode;
            std::ifstream vShaderFile;
            std::ifstream fShaderFile;

            // Makes sure ifstream objects can throw excpetions
            vShaderFile.exceptions(std::ifstream::failbit | std::fstream::badbit);
            vShaderFile.exceptions(std::ifstream::failbit | std::fstream::badbit);

            try {
                // Open files
                vShaderFile.open(vertexPath);
                fShaderFile.open(fragmentPath);
                std::stringstream vShaderStream, fShaderStream;

                // Read file buffers into streams
                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();

                // Convert stream into string
                vertexCode = vShaderStream.str();
                fragmentCode = fShaderStream.str();
            }
            catch (std::ifstream::failure& e) {
                std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
            }

            const char* vShaderCode = vertexCode.c_str();
            const char* fShaderCode = fragmentCode.c_str();

            // Compile shaders
            unsigned int vertex, fragment;

            // Vertex Shader
            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            glCompileShader(vertex);
            checkCompileErrors(vertex, "VERTEX");

            // Fragment Shader
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            glCompileShader(fragment);
            checkCompileErrors(fragment, "FRAGMENT");

            // Shader Program
            ID = glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment);
            glLinkProgram(ID);
            checkCompileErrors(ID, "PROGRAM");
        }

        // Activate shader
        void use() {
            glUseProgram(ID);
        }

        // Utility uniform functions for matrix maths and lighting
        void setInt(const std::string &name, int value) const {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }
        void setFloat(const std::string &name, float value) const {
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
        }
        void setVec3(const std::string &name, const glm::vec3 &value) const {
            glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
        }
        void setMat4(const std::string &name, const glm::mat4 &mat) const {
            glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }

    private: 
        // Utility function for checking shader compilation/linking errors
        void checkCompileErrors(unsigned int shader, std::string type) {
            int success;
            char infoLog[1024];
            if (type != "PROGRAM") {
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                if (!success) {
                    glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                    std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << ERROR_LINE << std::endl;
                }
            } else {
                glGetProgramiv(shader, GL_LINK_STATUS, &success);
                if (!success) {
                    glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                    std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << ERROR_LINE << std::endl;
                }
            }
        }
};

#endif
