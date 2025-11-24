cmake_minimum_required(VERSION 3.25)

project(PyNativeGuard
    VERSION 0.1.0
    LANGUAGES CXX C
)

# --- Compiler settings
set(CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output Directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# --- Visibility & ABI Control ---
# Critical for LD_PRELOAD: hide everything except what we explicitly export.
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

# --- Dependencies ---
include(FetchContent)

FetchContent_Declare(
    concurrentqueue
    GIT_REPOSITORY https://github.com/cameron314/concurrentqueue.git
    GIT_TAG v1.0.4
)
FetchContent_MakeAvailable(concurrentqueue)

# --- Subdirectories ---
add_subdirectory(src)

# --- Internal Interface Targets ---
add_library(project_headers INTERFACE)
target_include_directories(project_headers INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(external_concurrentqueue INTERFACE)
target_include_directories(external_concurrentqueue INTERFACE
    ${concurrentqueue_SOURCE_DIR}
)