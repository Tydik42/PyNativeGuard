add_library(pynativeguard_agent SHARED
    dll_main.cpp
)

target_link_libraries(pynativeguard_agent PRIVATE
    png_core
    \$<TARGET_OBJECTS:png_hooks>
)
