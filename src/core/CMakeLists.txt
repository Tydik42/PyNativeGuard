add_library(png_core STATIC
    dummy.cpp
)
target_link_libraries(png_core PUBLIC 
    png_headers
    vendor_concurrentqueue
    fmt::fmt
)
