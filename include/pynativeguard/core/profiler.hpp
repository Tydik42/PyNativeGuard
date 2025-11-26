#pragma once

#include "event.hpp"

#include "concurrentqueue.h" 

#include <thread>
#include <atomic>

namespace png::core {

class Profiler {
public:
    static Profiler& instance();

    void submit(Event evt);

  private:
    Profiler();
    ~Profiler();

    void open_trace_file();
    void close_trace_file();

    void worker_routine(std::stop_token st);

private:
  moodycamel::ConcurrentQueue<Event> queue_;

  std::jthread worker_thread_;

  std::atomic<uint64_t> total_events_{0};
  FILE *trace_file_ = nullptr;
};

} // namespace png::core
