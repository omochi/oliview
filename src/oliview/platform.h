#pragma once

#ifdef __APPLE__
#   define OLIVIEW_MACOS 1
#endif

#ifdef _WIN32
#   define OLIVIEW_WINDOWS 1
#endif

#ifdef OLIVIEW_MACOS
#   define OLIVIEW_PRETTY_FUNCTION __PRETTY_FUNCTION__
#   define OLIVIEW_NO_RETURN __attribute__((noreturn))
#   define OLIVIEW_PRINTF_LIKE(f, a) __attribute__((__format__ (__printf__, f, a)))
#endif
