#pragma once

#include "./dependency.h"

#define OLIVIEW_GL_ASSERT_NO_ERROR() \
oliview::GLAssertNoError(RHETORIC_PRETTY_FUNCTION, __FILE__, __LINE__)

namespace oliview {
    std::vector<GLenum> GLGetErrors();
    std::string GLErrorToString(GLenum error);
    void GLAssertNoError(const char * func,
                         const char * file,
                         int line);
}
