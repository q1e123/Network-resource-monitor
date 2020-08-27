#ifndef BUILD_OPTS_COMMUNICATION_PROTOCOL_H
#define BUILD_OPTS_COMMUNICATION_PROTOCOL_H

#if defined(_MSC_VER)
# ifdef COMMUNICATION_PROTOCOL_SHARED
#  define COMMUNICATION_PROTOCOL_IMPEXP __declspec(dllexport)
# else
#  define COMMUNICATION_PROTOCOL_IMPEXP __declspec(dllimport)
# endif
#elif defined(_GCC)
# ifdef COMMUNICATION_PROTOCOL_SHARED
#  define COMMUNICATION_PROTOCOL_IMPEXP __attribute__((visibility("default")))
# else
#  define COMMUNICATION_PROTOCOL_IMPEXP 
# endif
#else
#  define COMMUNICATION_PROTOCOL_IMPEXP
#endif

#endif