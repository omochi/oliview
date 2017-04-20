#pragma once

#include "./platform.h"

#include <cstdlib>
#include <cmath>
#include <vector>
#include <list>
#include <string>
#include <atomic>
#include <mutex>
#include <iostream>
#include <memory>

#ifdef OLIVIEW_MACOS
#   include <unistd.h>
#   include <dirent.h>
#endif

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


