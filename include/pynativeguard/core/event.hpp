#pragma once
#include <cstdint>

namespace png::core {

struct Event {
    uint64_t timestamp_ns;
    const char* format;
};

} // namespace png::core
