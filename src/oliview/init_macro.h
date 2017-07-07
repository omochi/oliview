#pragma once

#include "./dependency.h"

#define OLIVIEW_INIT(type, ...) OLIVIEW_INIT_NAME(type, Init, __VA_ARGS__)

#define OLIVIEW_INIT_NAME(type, init, ...) \
rhetoric::Let(RHETORIC_NEW(type), [&](auto thiz) { \
    thiz->init(__VA_ARGS__); \
    return thiz; \
})
