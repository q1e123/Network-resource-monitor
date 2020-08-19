
#if defined(_MSC_VER)
# ifdef UTIL_DLL
#  define UTILS_IMPEXP __declspec(dllexport)
# else
#  define UTILS_IMPEXP __declspec(dllimport)
# endif //UTIL_DLL
#elif defined(_GCC)
# ifdef UTIL_DLL
#  define UTILS_IMPEXP __attribute__((visibility("default")))
# else
#  define UTILS_IMPEXP 
# endif //UTIL_DLL
#else
#  define UTILS_IMPEXP
#endif
