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

#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLEXT
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <GLFW/glfw3.h>
#pragma clang diagnostic pop

#include <nanovg.h>

#define NANOVG_GL3
#include <nanovg_gl.h>
#include <nanovg_gl_utils.h>


