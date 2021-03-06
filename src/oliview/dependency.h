#pragma once

//  std
#include <cmath>
#include <list>
#include <chrono>

//  rhetoric
#include <rhetoric/rhetoric.h>

namespace oliview {
    using namespace rhetoric;
}

//  GLFW
#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <GLFW/glfw3.h>
#pragma clang diagnostic pop

//  nanovg
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#pragma clang diagnostic ignored "-Wshadow"
#pragma clang diagnostic ignored "-Wsign-conversion"

#include <nanovg.h>

#ifndef NANOVG_GL3_IMPLEMENTATION
#   define NANOVG_GL3
#endif
#include <nanovg_gl.h>
#include <nanovg_gl_utils.h>

#pragma clang diagnostic pop

