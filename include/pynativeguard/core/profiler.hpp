#pragma once

#include "event.hpp"

#include "concurrentqueue.h" 

#include <thread>
#include <atomic>

namespace png::core {

class Profiler {
public:
    static Profiler& instance();

    void submit(const char* format);

private:
    Profiler();
    ~Profiler();

    void worker_routine(std::stop_token st);

private:
    moodycamel::ConcurrentQueue<Event> queue_;
    
    std::atomic<uint64_t> total_events_{0};

    std::jthread worker_thread_;
};

} // namespace png::core
