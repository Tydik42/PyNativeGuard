# PyNativeGuard

PyNativeGuard is a high-performance, non-intrusive ABI-level profiler for Python/C++ interoperability. It uses `LD_PRELOAD` injection to intercept Python C-API calls (such as `PyArg_ParseTuple`) to measure the overhead of language boundary crossings and argument serialization.

## Features

*   **C++20 Architecture:** Built using modern standards (Concepts, JThread, Semaphores).
*   **Zero-Overhead Hot Path:** Uses `moodycamel::ConcurrentQueue` for lock-free event logging.
*   **Non-Blocking:** Data collection occurs without acquiring the Global Interpreter Lock (GIL).
*   **Visualization:** Exports performance data to **Chrome Tracing (JSON)** format.

## Requirements

*   Linux (x86_64)
*   CMake 3.25 or higher
*   C++20 compatible compiler (GCC 10+, Clang 12+)
*   Python 3.10+

## Build Instructions

1.  **Configure the project:**
    ```bash
    cmake -B build -DCMAKE_BUILD_TYPE=Release
    ```

2.  **Build the agent:**
    ```bash
    cmake --build build --parallel
    ```

## Usage

The project includes a helper tool to handle library injection automatically.

**Run a specific Python script:**
```bash
python3 tools/run.py tests/demo/demo.py
