add_library(pynativeguard SHARED
    hooks.cpp
)

target_compile_features(pynativeguard PRIVATE cxx_std_20)

target_link_libraries(pynativeguard PRIVATE
    project_headers
    external_concurrentqueue
)

# ABI: Python symbols resolution
if(UNIX AND NOT APPLE)
    target_link_options(pynativeguard PRIVATE "LINKER:--allow-shlib-undefined")
endif()

message(STATUS "PyNativeGuard lib will be built in: \${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")