#include <chrono>
#include <iostream>

#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

template <typename T> class cvQueue {
private:
  std::queue<T> queue;
  std::mutex m;
  std::condition_variable cv;
  bool notified = false;
  bool finished = false;

public:
  void send(T t) {
    std::unique_lock<std::mutex> lock(m);
    this->queue.push(t);
    notified = true;
    cv.notify_one();
  }
  bool recv(T &t) {
    std::unique_lock<std::mutex> lock(m);
    if (!this->finished) {
      cv.wait(lock, [this] { return this->notified; });
      if (!this->queue.empty()) {
        t = this->queue.front();
        this->queue.pop();
        notified = false;
        return true;
      }
    }
    return false;
  }
  void close() {
    std::unique_lock<std::mutex> lock(m);
    this->finished = true;
    this->notified = true;
    cv.notify_one();
  }
};

int main(int argc, char *argv[]) {
  cvQueue<int> q;
  std::thread t([&]() {
    for (int i = 0; i < 10; i++) {
      q.send(i);
      std::cout << "producing: " << i << '\n';
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    q.close();
  });

  int get;
  while (q.recv(get)) {
    std::cout << "receiving: " << get << '\n';
  }

  // t.join();
  return 0;
}
