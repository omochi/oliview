#pragma once

#include "./dependency.h"
#include "./string_format.h"
#include "./string_function.h"
#include "./assert.h"

#define OLIVIEW_GL_ASSERT_NO_ERROR() \
oliview::GLAssertNoError(OLIVIEW_PRETTY_FUNCTION, __FILE__, __LINE__)

namespace oliview {
    std::vector<GLenum> GLGetErrors();
    std::string GLErrorToString(GLenum error);
    void GLAssertNoError(const char * func,
                         const char * file,
                         int line);
}
