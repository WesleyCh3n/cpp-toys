#include <iostream>

#include <condition_variable>
#include <mutex>
#include <thread>

int main(int argc, char *argv[]) {
  {
    // without mutex protect
    std::mutex mtx;
    int a = 0;
    int times = 1000000;
    std::thread t1(
        [times](int &n, std::mutex &mtx) {
          for (int i = 0; i < times; i++) {
            n += 1;
          }
        },
        std::ref(a), std::ref(mtx));

    std::thread t2(
        [times](int &n, std::mutex &mtx) {
          for (int i = 0; i < times; i++) {
            n += 1;
          }
        },
        std::ref(a), std::ref(mtx));

    t1.join();
    t2.join();
    // the value may not be 200000 because two thread racing variable a
    std::cout << a << '\n';
  }
  {
    // with mutex protect
    std::mutex mtx;
    int a = 0;
    int times = 1000000;
    std::thread t1(
        [times](int &n, std::mutex &mtx) {
          for (int i = 0; i < times; i++) {
            {
              std::unique_lock<std::mutex> guard(mtx); // guard this scope
              n += 1;
            }
            // std::this_thread::sleep_for(std::chrono::milliseconds(100));
          }
        },
        std::ref(a), std::ref(mtx));

    std::thread t2(
        [times](int &n, std::mutex &mtx) {
          for (int i = 0; i < times; i++) {
            std::unique_lock<std::mutex> guard(mtx);
            n += 1;
          }
        },
        std::ref(a), std::ref(mtx));

    t1.join();
    t2.join();
    std::cout << a << '\n';
  }

  {
    std::condition_variable cv;
    std::mutex m;
    std::thread worker(
        [](std::mutex &m, std::condition_variable &cv) {
          std::unique_lock<std::mutex> guard(m);
        },
        std::ref(m), std::ref(cv));
  }
  return 0;
}
