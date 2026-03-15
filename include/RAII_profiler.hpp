#pragma once

#include <chrono>
#include <iostream>
#include <string>

template <typename Duration = std::chrono::microseconds> class RAII_Profiler {
public:
  explicit RAII_Profiler(std::string name)
      : name_(std::move(name)), start_(std::chrono::steady_clock::now()) {}

  ~RAII_Profiler() {
    auto end = std::chrono::steady_clock::now();

    auto duration = std::chrono::duration_cast<Duration>(end - start_);

    std::cout << name_ << " took " << duration.count() << " " << unitName()
              << "\n";
  }

private:
  std::string name_;
  std::chrono::time_point<std::chrono::steady_clock> start_;

  static const char *unitName() {
    if constexpr (std::is_same_v<Duration, std::chrono::microseconds>)
      return "us";
    else if constexpr (std::is_same_v<Duration, std::chrono::milliseconds>)
      return "ms";
    else if constexpr (std::is_same_v<Duration, std::chrono::nanoseconds>)
      return "ns";
    else
      return "units";
  }
};