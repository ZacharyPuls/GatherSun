//
// Created by zach on 2021-09-20.
//

#include "Texture.h"

#include <iostream>

namespace gathersun::render {

    Texture2D::Texture2D(const std::string &filename, bool flipVertically, bool generateMipmaps) : filename_(filename) {
        int width;
        int height;
        int numChannels;

        stbi_set_flip_vertically_on_load(flipVertically);
        unsigned char *pixels = stbi_load(filename.c_str(), &width, &height, &numChannels, 0);

        if (pixels == nullptr) {
            // TODO: formalize debug/logging
            std::cerr << "Failed to load image file with filename " << filename << "." << std::endl;
        }

        // TODO: just hackily adding numChannels == 1 to ternary, really should make this cover all cases.
        GLint internalFormat = numChannels == 4 ? GL_RGBA : numChannels == 1 ? GL_RED : GL_RGB;
        GLenum format = numChannels == 4 ? GL_RGBA : numChannels == 1 ? GL_RED : GL_RGB;

        glGenTextures(1, &id_);
        this->Bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        if (generateMipmaps) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        } else {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);
        if (generateMipmaps) {
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        stbi_image_free(pixels);

        width_ = width;
        height_ = height;
    }

    Texture2D::~Texture2D() {

    }

    void Texture2D::Bind() const {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, id_);
    }

    Texture2D::Texture2D(GLsizei width, GLsizei height, GLint internalFormat, GLenum format, GLenum type,
                         const void *data)
            : width_(width), height_(height) {
        glGenTextures(1, &id_);
        this->Bind();
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width_, height_, 0, format, type, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    void Texture2D::Update(GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type,
                           const void *data) {
        glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset, yoffset, width, height, format, type, data);
    }

    void Texture2D::SetOptions(GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum magFilter) {
        this->Bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    }

    GLsizei Texture2D::GetWidth() const {
        return width_;
    }

    GLsizei Texture2D::GetHeight() const {
        return height_;
    }

    const std::string &Texture2D::GetFilename() const {
        return filename_;
    }
}
