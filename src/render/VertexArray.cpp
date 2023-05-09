//
// Created by zach on 2021-09-20.
//

#include "VertexArray.h"

namespace gathersun::render {

    VertexArray::VertexArray() : id_{} {
        glCreateVertexArrays(1, &id_);
    }

    VertexArray::~VertexArray() {
        glDeleteVertexArrays(1, &id_);
    }

    void VertexArray::Bind() const {
        glBindVertexArray(id_);
    }

    void VertexArray::Draw(GLenum mode, GLint first, GLsizei count) const {
        Bind();
        glDrawArrays(mode, first, count);
    }
}
