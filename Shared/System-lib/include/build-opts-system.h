#ifndef BUILD_OPTS_SYSTEM_H
#define BUILD_OPTS_SYSTEM_H

#if defined(_MSC_VER)
# ifdef SYSTEM_SHARED
#  define SYSTEM_IMPEXP __declspec(dllexport)
# else
#  define SYSTEM_IMPEXP __declspec(dllimport)
# endif
#elif defined(_GCC)
# ifdef SYSTEM_SHARED
#  define SYSTEM_IMPEXP __attribute__((visibility("default")))
# else
#  define SYSTEM_IMPEXP 
# endif
#else
#  define SYSTEM_IMPEXP
#endif

#endif