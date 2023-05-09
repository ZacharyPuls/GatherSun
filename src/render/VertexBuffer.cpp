//
// Created by zach on 2021-09-20.
//

#include "VertexBuffer.h"

namespace gathersun::render {

    VertexBuffer::VertexBuffer(GLenum target, GLenum usage) : id_{}, target_(target), usage_(usage) {
        glCreateBuffers(1, &id_);
    }

    VertexBuffer::~VertexBuffer() {
        glDeleteBuffers(1, &id_);
    }

    void VertexBuffer::Bind() const {
        glBindBuffer(target_, id_);
    }

    void VertexBuffer::SetData(GLsizeiptr size, void *data) const {
        Bind();
        glBufferData(target_, size, data, usage_);
    }

    void VertexBuffer::SetSubData(GLintptr offset, GLsizeiptr size, void *data) const {
        Bind();
        glBufferSubData(target_, offset, size, data);
    }

    void VertexBuffer::VertexAttribPointer(GLint index, GLint size, GLenum type, GLsizei stride, GLsizei offset) const {
        Bind();
        glVertexAttribPointer(index, size, type, GL_FALSE, stride, reinterpret_cast<const void *>(offset));
        glEnableVertexAttribArray(index);
    }
}
