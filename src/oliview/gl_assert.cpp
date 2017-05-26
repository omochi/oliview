#include "./gl_assert.h"

namespace oliview {
    std::vector<GLenum> GLGetErrors() {
        std::vector<GLenum> ret;
        while (true) {
            GLenum error = glGetError();
            if (error == GL_NO_ERROR) {
                break;
            }
            ret.push_back(error);
        }
        return ret;
    }

    std::string GLErrorToString(GLenum error) {
        switch (error) {
            case GL_NO_ERROR:
                return "GL_NO_ERROR";
            case GL_INVALID_ENUM:
                return "GL_INVALID_ENUM";
            case GL_INVALID_VALUE:
                return "GL_INVALID_VALUE";
            case GL_INVALID_OPERATION:
                return "GL_INVALID_OPERATION";
            case GL_OUT_OF_MEMORY:
                return "GL_OUT_OF_MEMORY";
            default:
                return "unknown error";
        }
    }

    void GLAssertNoError(const char * func,
                         const char * file,
                         int line)
    {
        auto errors = GLGetErrors();
        if (errors.size() == 0) {
            return;
        }

        std::vector<std::string> strs;
        for (auto error : errors) {
            strs.push_back(Format("%s(0x%04x)",
                                  GLErrorToString(error).c_str(),
                                  error));
        }
        std::string err_str = Join(strs, ", ");

        std::string str = Format("OpenGL error: [%s], func=[%s], file=[%s], line=%d",
                                 err_str.c_str(),
                                 func,
                                 file,
                                 line);
        RHETORIC_FATAL(str);
    }
}
