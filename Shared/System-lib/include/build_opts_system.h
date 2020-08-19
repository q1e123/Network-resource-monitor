
#if defined(_MSC_VER)
# ifdef SYSTEM_DLL
#  define SYSTEMS_IMPEXP __declspec(dllexport)
# else
#  define SYSTEMS_IMPEXP __declspec(dllimport)
# endif //SYSTEM_DLL
#elif defined(_GCC)
# ifdef SYSTEM_DLL
#  define SYSTEMS_IMPEXP __attribute__((visibility("default")))
# else
#  define SYSTEMS_IMPEXP 
# endif //SYSTEM_DLL
#else
#  define SYSTEMS_IMPEXP
#endif
