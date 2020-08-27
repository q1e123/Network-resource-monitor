#ifndef BUILD_OPTS_UTILS_H
#define BUILD_OPTS_UTILS_H

#if defined(_MSC_VER)
# ifdef UTILS_SHARED
#  define UTILS_IMPEXP __declspec(dllexport)
# else
#  define UTILS_IMPEXP __declspec(dllimport)
# endif
#elif defined(_GCC)
# ifdef UTILS_SHARED
#  define UTILS_IMPEXP __attribute__((visibility("default")))
# else
#  define UTILS_IMPEXP 
# endif
#else
#  define UTILS_IMPEXP
#endif

#endif