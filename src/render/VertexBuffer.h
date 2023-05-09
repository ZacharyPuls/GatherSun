//
// Created by zach on 2021-09-20.
//

#ifndef GATHERSUN_RENDER_VERTEXBUFFER_H
#define GATHERSUN_RENDER_VERTEXBUFFER_H

#include "Core.h"

namespace gathersun::render {

    class VertexBuffer {
    public:
        VertexBuffer(GLenum target, GLenum usage);

        VertexBuffer(const VertexBuffer &&other) = delete;

        ~VertexBuffer();

        void Bind() const;

        void SetData(GLsizeiptr size, void *data) const;

        void SetSubData(GLintptr offset, GLsizeiptr size, void *data) const;

        // TODO: find a better way to do this, keep OpenGL vertex specification-specific code out of VertexBuffer
        void VertexAttribPointer(GLint index, GLint size, GLenum type, GLsizei stride, GLsizei offset) const;

    private:
        GLuint id_;
        GLenum target_;
        GLenum usage_;
    };
}

#endif // GATHERSUN_RENDER_VERTEXBUFFER_H
