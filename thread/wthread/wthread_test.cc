#include "wthread.h"

int main(int argc, char *argv[]) {
  LOG("Entering main...");
  WThread w;
  w.Start();
  w.Start();
  std::this_thread::sleep_for(std::chrono::seconds(2));

  w.Pause();
  std::this_thread::sleep_for(std::chrono::seconds(2));

  w.Resume();
  std::this_thread::sleep_for(std::chrono::seconds(2));

  w.Pause();
  std::this_thread::sleep_for(std::chrono::seconds(2));

  w.Resume();
  std::this_thread::sleep_for(std::chrono::seconds(2));

  // w.Stop(); // destructure will stop the thread too
  LOG("Exiting main...");
  return 1;
}
