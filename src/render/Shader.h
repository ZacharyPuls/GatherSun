//
// Created by zach on 2021-09-25.
//

#ifndef GATHERSUN_SHADER_H
#define GATHERSUN_SHADER_H

#include "Core.h"

namespace gathersun::render {

    class Shader {
    public:
        explicit Shader(const std::string &filename);

        ~Shader();

        void Bind() const;

        void UniformMat4(GLint location, glm::mat4 value) const;

        void UniformTexture(GLint location, GLint textureUnit = 0) const;

        void UniformVec2(GLint location, glm::vec2 value) const;

        void UniformVec3(GLint location, glm::vec3 value) const;

        void UniformVec4(GLint location, glm::vec4 value) const;

    private:
        GLuint id_ = 0;
    };
}

#endif // GATHERSUN_SHADER_H
