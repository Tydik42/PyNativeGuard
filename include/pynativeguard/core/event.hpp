#pragma once
#include <cstdint>

#include "types.hpp"

namespace png::core {

enum class EventType : uint8_t {
  ArgParse,
  FunctionCall,
  GilAcquire,
  GilRelease
};

struct Event {
  timestamp_t timestamp;
  tid_t tid;
  EventType type;

  timestamp_t duration;
  const char *name;
};

} // namespace png::core
