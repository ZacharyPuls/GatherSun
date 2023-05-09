//
// Created by zach on 2021-09-20.
//

#ifndef GATHERSUN_RENDER_VERTEXARRAY_H
#define GATHERSUN_RENDER_VERTEXARRAY_H

#include "Core.h"

namespace gathersun::render {

    class VertexArray {
    public:
        VertexArray();

        ~VertexArray();

        void Bind() const;

        void Draw(GLenum mode, GLint first, GLsizei count) const;

    private:
        GLuint id_;
    };
}

#endif // GATHERSUN_RENDER_VERTEXARRAY_H
