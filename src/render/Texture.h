//
// Created by zach on 2021-09-20.
//

#ifndef GATHERSUN_RENDER_TEXTURE_H
#define GATHERSUN_RENDER_TEXTURE_H

#include "Core.h"

#include <array>
#include <string>

namespace gathersun::render {

    class Texture {
    public:
        virtual ~Texture() = default;

        virtual void Bind() const = 0;
    };

    class Texture2D : public Texture {
    public:
        Texture2D() = default;

        explicit Texture2D(const std::string &filename, bool flipVertically = false, bool generateMipmaps = false);

        explicit Texture2D(GLsizei width, GLsizei height, GLint internalFormat, GLenum format, GLenum type,
                           const void *data);

        ~Texture2D() override;

        void Bind() const override;

        void Update(GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type,
                    const void *data);

        void SetOptions(GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum magFilter);

        GLsizei GetWidth() const;

        GLsizei GetHeight() const;

        const std::string& GetFilename() const;

    private:
        GLuint id_ = 0;
        GLsizei width_;
        GLsizei height_;
        std::string filename_;
    };
}

#endif // GATHERSUN_RENDER_TEXTURE_H
