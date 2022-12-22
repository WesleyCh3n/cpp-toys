#include <iostream>
#include <thread>

#include "../Timer.h"

void func1() {
  std::cout << "running func1...\n";
  std::this_thread::sleep_for(std::chrono::seconds(1));
  return;
}
void func2(int i) {
  std::cout << "running func2, i: " << i << "...\n";
  std::this_thread::sleep_for(std::chrono::seconds(2));
  return;
}

int main(int argc, char *argv[]) {
  Timer t;
  std::thread t1(func1);
  std::thread t2(func2, 2);
  std::cout << "main...\n";
  t1.join();
  t2.join();
  std::cout << "all complete...\n";
  return 0;
}
