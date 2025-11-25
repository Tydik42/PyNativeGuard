#include "pynativeguard/core/profiler.hpp"
#include <chrono>
#include <cstdio>
#include <thread>

namespace png::core {

Profiler& Profiler::instance() {
    static Profiler inst;
    return inst;
}

Profiler::Profiler() {
    worker_thread_ = std::jthread([this](std::stop_token st) {
        this->worker_routine(st);
    });
}

Profiler::~Profiler() {
    std::fprintf(stderr, "\n[PyNativeGuard] Shutdown. Total events processed: %lu\n", 
                 total_events_.load());
}

void Profiler::submit(const char* format) {
    auto now = std::chrono::steady_clock::now();
    uint64_t ts = std::chrono::duration_cast<std::chrono::nanoseconds>(
        now.time_since_epoch()
    ).count();

    queue_.enqueue(Event{ts, format});
}

void Profiler::worker_routine(std::stop_token st) {
    Event events[32]; 
    
    while (!st.stop_requested()) {
        size_t count = queue_.try_dequeue_bulk(events, 32);
        
        if (count == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }

        total_events_.fetch_add(count, std::memory_order_relaxed);
        
        for (size_t i = 0; i < count; ++i) {
            std::fprintf(stderr, "[Profiling] TS=%lu | FMT='%s'\n", 
                         events[i].timestamp_ns, events[i].format);
        }
        std::fflush(stderr);
    }
}

} // namespace png::core
