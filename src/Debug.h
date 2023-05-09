//
// Created by zach on 2023-04-22.
//

#ifndef GATHERSUN_DEBUG_H
#define GATHERSUN_DEBUG_H

#include "Core.h"

namespace gathersun {

    void GlfwErrorCallback(int error, const char *description);

    void GLAPIENTRY OpenglDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                        const GLchar *message, const void *user_param);
}

#endif // GATHERSUN_DEBUG_H
