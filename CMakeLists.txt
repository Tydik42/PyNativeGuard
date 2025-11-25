cmake_minimum_required(VERSION 3.25)
project(PyNativeGuard VERSION 0.1.0 LANGUAGES CXX C)

# --- Global Settings ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Output paths
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# --- Dependencies ---
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(dependencies)

# --- Internal Interfaces ---
add_library(png_headers INTERFACE)
target_include_directories(png_headers INTERFACE include)

# --- Subdirectories ---
add_subdirectory(src/core)
add_subdirectory(src/hooks)
add_subdirectory(src/agent)

message(STATUS "PyNativeGuard: Infrastructure Ready.")
