#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "pynativeguard/macros.hpp"
#include "pynativeguard/core/profiler.hpp" 

#include <dlfcn.h>
#include <cstdarg>
#include <atomic>
#include <cstdio>
#include <utility>

using PyObject = void;
using PyArg_VaParse_t = int (*)(PyObject*, const char*, va_list);

constinit std::atomic<PyArg_VaParse_t> g_real_PyArg_VaParse { nullptr };

template<typename T>
T get_symbol(std::atomic<T>& cache, const char* symbol_name) {
    T ptr = cache.load(std::memory_order_acquire);
    if (__builtin_expect(ptr != nullptr, 1)) return ptr;

    void* resolved = dlsym(RTLD_DEFAULT, symbol_name);
    if (!resolved) {
        fprintf(stderr, "[PyNativeGuard] FATAL: Symbol '%s' not found!\n", symbol_name);
        __builtin_trap();
    }
    ptr = reinterpret_cast<T>(resolved);
    cache.store(ptr, std::memory_order_release);
    return ptr;
}

extern "C" {

PNG_EXPORT
int PyArg_ParseTuple(PyObject* args, const char* format, ...) {
    png::core::Profiler::instance().submit(format);

    va_list vargs;
    va_start(vargs, format);

    auto real_func = get_symbol(g_real_PyArg_VaParse, "PyArg_VaParse");
    int result = real_func(args, format, vargs);

    va_end(vargs);
    return result;
}

} // extern "C"
