add_library(png_hooks OBJECT
    python_api.cpp
)
target_link_libraries(png_hooks PRIVATE 
    png_headers 
    png_core
)

if(UNIX)
    target_link_options(png_hooks PRIVATE "LINKER:--allow-shlib-undefined")
endif()