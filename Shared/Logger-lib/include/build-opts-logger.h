#ifndef BUILD_OPTS_LOGGER_H
#define BUILD_OPTS_LOGGER_H

#if defined(_MSC_VER)
# ifdef LOGGER_SHARED
#  define LOGGER_IMPEXP __declspec(dllexport)
# else
#  define LOGGER_IMPEXP __declspec(dllimport)
# endif
#elif defined(_GCC)
# ifdef LOGGER_SHARED
#  define LOGGER_IMPEXP __attribute__((visibility("default")))
# else
#  define LOGGER_IMPEXP 
# endif
#else
#  define LOGGER_IMPEXP
#endif

#endif