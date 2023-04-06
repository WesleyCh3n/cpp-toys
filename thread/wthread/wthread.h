#ifndef WTHREAD_H
#define WTHREAD_H

#include <iostream>

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <optional>
#include <thread>

#define LOG(x) std::cout << x << '\n';

class WThread {
public:
  WThread();
  ~WThread();
  void Start();
  void Pause();
  void Resume();
  void Stop();

private:
  void Run();
  void ProcessFunction();
  std::optional<std::thread> thread_;
  std::atomic_bool stop_;
  std::atomic_bool pause_;
  std::condition_variable cv_;
  std::mutex mutex_;
};

#endif
