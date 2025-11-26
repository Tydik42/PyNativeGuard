#pragma once

// OS Detection
#if defined(__linux__)
#define PNG_OS_LINUX
#elif defined(_WIN32)
#define PNG_OS_WINDOWS
#endif

// Visibility (Export/Import)
#if defined(_MSC_VER)
    #define PNG_EXPORT __declspec(dllexport)
#else
    #define PNG_EXPORT __attribute__((visibility("default")))
#endif

// Optimization Hints
#define PNG_INLINE inline __attribute__((always_inline))
#define PNG_NOINLINE __attribute__((noinline))
#define PNG_LIKELY(x) __builtin_expect(!!(x), 1)
#define PNG_UNLIKELY(x) __builtin_expect(!!(x), 0)
