include(FetchContent)

# --- Moodycamel ConcurrentQueue ---
FetchContent_Declare(
    concurrentqueue
    GIT_REPOSITORY https://github.com/cameron314/concurrentqueue.git
    GIT_TAG master
)
FetchContent_MakeAvailable(concurrentqueue)

if(NOT TARGET vendor_concurrentqueue)
    add_library(vendor_concurrentqueue INTERFACE)
    target_include_directories(vendor_concurrentqueue INTERFACE ${concurrentqueue_SOURCE_DIR})
endif()

# --- fmt ---
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 10.1.1
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(fmt)