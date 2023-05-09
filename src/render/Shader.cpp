//
// Created by zach on 2021-09-25.
//

#include "Shader.h"

#include <fstream>
#include <iostream>

namespace gathersun::render {

    static GLuint CompileShader(GLenum type, const char *source) {
        GLuint id = glCreateShader(type);
        glShaderSource(id, 1, &source, nullptr);
        glCompileShader(id);

        GLint status = GL_FALSE;
        glGetShaderiv(id, GL_COMPILE_STATUS, &status);

        if (status == GL_FALSE) {
            GLint infoLogLength = 0;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);

            char *infoLog = new char[infoLogLength];
            glGetShaderInfoLog(id, infoLogLength, &infoLogLength, infoLog);

            // TODO: formalize debug/logging
            std::cerr << "Failed to compile shader. Error log: " << std::endl << infoLog << std::endl;

            delete[] infoLog;

            return 0;
        }

        return id;
    }

    Shader::Shader(const std::string &filename) {
        id_ = glCreateProgram();

        // TODO: don't expect just a vertexShaderSource and fragmentShaderSource shader stage, iterate through each #stage segment, compile each stage separately, and then link into program.

        std::ifstream file(filename);
        if (!file.is_open()) {
            // TODO: formalize debug/logging
            std::cerr << "Could not open shader with filename " << filename << "." << std::endl;
            return;
        }

        std::string source = "";

        std::string line;
        while (std::getline(file, line)) {
            source += line + '\n';
        }

        std::string vertexShaderSource =
                source.substr(source.find("#stage vertex"), source.find("#stage fragment"));
        vertexShaderSource.erase(0, vertexShaderSource.find('\n'));
        std::string fragmentShaderSource = source.substr(source.find("#stage fragment"), std::string::npos);
        fragmentShaderSource.erase(0, fragmentShaderSource.find('\n'));

        GLuint vertexShaderId = CompileShader(GL_VERTEX_SHADER, vertexShaderSource.c_str());
        GLuint fragmentShaderId = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource.c_str());

        id_ = glCreateProgram();

        glAttachShader(id_, vertexShaderId);
        glAttachShader(id_, fragmentShaderId);
        glLinkProgram(id_);

        GLint status = GL_FALSE;
        glGetProgramiv(id_, GL_LINK_STATUS, &status);

        if (status == GL_FALSE) {
            GLint infoLogLength = 0;
            glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &infoLogLength);

            char *infoLog = new char[infoLogLength];
            glGetProgramInfoLog(id_, infoLogLength, &infoLogLength, infoLog);

            // TODO: formalize debug/logging
            std::cerr << "Failed to link shader program. Error log: " << std::endl << infoLog << std::endl;

            delete[] infoLog;
        }

        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);
    }

    Shader::~Shader() {
        glDeleteProgram(id_);
    }

    void Shader::Bind() const {
        glUseProgram(id_);
    }

    void Shader::UniformMat4(GLint location, const glm::mat4 value) const {
        glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat *) glm::value_ptr(value));
    }

    void Shader::UniformTexture(GLint location, GLint textureUnit) const {
        glUniform1i(location, textureUnit);
    }

    void Shader::UniformVec2(GLint location, glm::vec2 value) const {
        glUniform2f(location, value.x, value.y);
    }

    void Shader::UniformVec3(GLint location, glm::vec3 value) const {
        glUniform3f(location, value.x, value.y, value.z);
    }

    void Shader::UniformVec4(GLint location, glm::vec4 value) const {
        glUniform4fv(location, 4, glm::value_ptr(value));
    }
}
