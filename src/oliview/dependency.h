#pragma once

#include <rhetoric/rhetoric.h>

namespace oliview {
    using namespace rhetoric;
}

#include <cmath>
#include <list>
#include <mutex>

#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <GLFW/glfw3.h>
#pragma clang diagnostic pop

#include <nanovg.h>

#ifndef NANOVG_GL3_IMPLEMENTATION
#   define NANOVG_GL3
#endif
#include <nanovg_gl.h>
#include <nanovg_gl_utils.h>

#include <stb_truetype.h>


