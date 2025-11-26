#include "pynativeguard/core/profiler.hpp"

#include <chrono>
#include <sys/syscall.h>
#include <unistd.h>
#include <vector>

#include <fmt/compile.h>
#include <fmt/core.h>

#include "pynativeguard/core/types.hpp"
#include "pynativeguard/macros.hpp"

namespace png::core {

static inline tid_t sys_get_tid() {
#ifdef PNG_OS_LINUX
  return static_cast<tid_t>(syscall(SYS_gettid));
#else
  return static_cast<tid_t>(
      std::hash<std::thread::id>{}(std::this_thread::get_id()));
#endif
}

static tid_t get_cached_tid() {
  static thread_local tid_t t_tid = 0;
  if (PNG_UNLIKELY(t_tid == 0)) {
    t_tid = sys_get_tid();
  }
  return t_tid;
}

Profiler& Profiler::instance() {
    static Profiler inst;
    return inst;
}

Profiler::Profiler() {
  open_trace_file();

  worker_thread_ =
      std::jthread([this](std::stop_token st) { this->worker_routine(st); });
}

Profiler::~Profiler() { close_trace_file(); }

void Profiler::open_trace_file() {
  // TODO: getenv("PNG_OUTPUT")
  trace_file_ = std::fopen("trace.json", "w");
  if (trace_file_) {
    fmt::print(trace_file_, "[\n");
  } else {
    fmt::print(stderr, "[PyNativeGuard] ERROR: Could not open trace.json!\n");
  }
}

void Profiler::close_trace_file() {
  if (trace_file_) {
    fmt::print(trace_file_, "\n{{}}]\n");
    std::fclose(trace_file_);
    trace_file_ = nullptr;

    fmt::print(stderr, "[PyNativeGuard] Trace saved. Events: {}\n",
               total_events_.load());
  }
}

void Profiler::submit(Event evt) {
  if (evt.tid == 0) {
    evt.tid = get_cached_tid();
  }

  if (evt.timestamp == 0) {
    auto now = std::chrono::steady_clock::now();
    evt.timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(
                        now.time_since_epoch())
                        .count();
  }

  queue_.enqueue(evt);
}

void Profiler::worker_routine(std::stop_token st) {
  constexpr size_t BATCH_SIZE = 32;
  Event events[BATCH_SIZE];
  tid_t pid = static_cast<tid_t>(getpid());
  bool is_first_write = true;

  while (!st.stop_requested()) {
    size_t count = queue_.try_dequeue_bulk(events, BATCH_SIZE);

    if (count == 0) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
      continue;
    }

    total_events_.fetch_add(count, std::memory_order_relaxed);

    if (!trace_file_)
      continue;

    for (size_t i = 0; i < count; ++i) {
      const auto &evt = events[i];

      if (!is_first_write) {
        std::fputc(',', trace_file_);
        std::fputc('\n', trace_file_);
      }
      is_first_write = false;

      timestamp_t ts_us = evt.timestamp / 1000;

      switch (evt.type) {
      case EventType::ArgParse:
        fmt::print(
            trace_file_,
            R"({{"name": "PyArg_ParseTuple", "cat": "c-api", "ph": "i", "ts": {}, "pid": {}, "tid": {}, "s": "t", "args": {{"fmt": "{}"}}}})",
            ts_us, pid, evt.tid, (evt.name ? evt.name : "null"));
        break;

      case EventType::FunctionCall:
        fmt::print(
            trace_file_,
            R"({{"name": "{}", "cat": "function", "ph": "X", "ts": {}, "dur": {}, "pid": {}, "tid": {}}})",
            (evt.name ? evt.name : "unknown"), ts_us, evt.duration / 1000, pid,
            evt.tid);
        break;

      default:
        break;
      }
    }
    std::fflush(trace_file_);
  }
}
} // namespace png::core
