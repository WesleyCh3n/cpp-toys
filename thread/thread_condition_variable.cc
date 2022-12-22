#include <iostream>

#include <condition_variable>
#include <mutex>
#include <thread>

int main(int argc, char *argv[]) {
  {
    std::cout << "Example 1\n";
    std::condition_variable cv;
    std::mutex m;
    bool ready = false;
    std::thread worker(
        [](std::mutex &m, std::condition_variable &cv, bool &ready) {
          std::unique_lock<std::mutex> lock(m);
          std::cout << "worker start waiting\n";
          cv.wait(lock, [&] { return ready; });
          std::cout << "worker continue\n";
        },
        std::ref(m), std::ref(cv), std::ref(ready));

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << "main notify\n";

    {
      std::unique_lock<std::mutex> lock(m);
      ready = true;
      cv.notify_one();
    }
    worker.join();
    std::cout << "main end\n";
  }
  {
    std::cout << "Example 2\n";
    std::condition_variable cv;
    std::mutex m;
    bool ready = false, data_finished = false;
    std::string data;
    std::thread worker(
        [&ready, &data_finished](std::mutex &m, std::condition_variable &cv,
                                 std::string &data) {
          std::unique_lock<std::mutex> lock(m);
          std::cout << "worker start waiting\n";
          cv.wait(lock, [&] { return ready; });

          std::cout << "worker continue\n";
          data += "some data";

          data_finished = true;
          std::cout << "worker finished\n";
          cv.notify_one();
        },
        std::ref(m), std::ref(cv), std::ref(data));

    {
      std::unique_lock<std::mutex> lock(m);
      ready = true;
      std::cout << "main notify\n";
      cv.notify_one();
    }

    {
      std::unique_lock<std::mutex> lock(m);
      cv.wait(lock, [&data_finished] { return data_finished; });
    }

    std::cout << "main got data: " << data << '\n';
    worker.join();
    std::cout << "main end\n";
  }
  return 0;
}
