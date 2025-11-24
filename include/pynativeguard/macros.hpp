#if defined(_MSC_VER)
    #define PNG_EXPORT __declspec(dllexport)
#else
    #define PNG_EXPORT __attribute__((visibility("default")))
#endif

#if defined(_MSC_VER)
    #define PNG_INLINE __forceinline
#else
    #define PNG_INLINE inline __attribute__((always_inline))
#endif