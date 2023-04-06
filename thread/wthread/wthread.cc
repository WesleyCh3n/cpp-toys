#include "wthread.h"

#include <stdexcept>

WThread::WThread() : stop_(false), pause_(false) {}
WThread::~WThread() { this->Stop(); }

void WThread::Start() {
  if (!this->thread_) {
    LOG("Start thread")
    this->thread_ = std::thread(&WThread::Run, this);
    this->stop_ = false;
    this->pause_ = false;
  } else {
    LOG("Already Start the thread. Skip!");
  }
}

void WThread::Pause() {
  if (this->thread_) {
    LOG("Pause thread")
    this->pause_ = true;
  }
}

void WThread::Resume() {
  if (this->thread_) {
    LOG("Resume thread")
    this->pause_ = false;
    this->cv_.notify_all();
  }
}

void WThread::Stop() {
  if (this->thread_ && !this->stop_) {
    LOG("Stop thread")
    this->pause_ = false;
    this->stop_ = true;
    this->cv_.notify_all();
    this->thread_->join();
    this->thread_ = {};
  }
}

void WThread::Run() {
  LOG("Enter Thread: " << std::this_thread::get_id());
  while (!this->stop_) {
    this->ProcessFunction();

    if (this->pause_) {
      // pause this thread
      std::unique_lock<std::mutex> locker(this->mutex_);
      this->cv_.wait(locker);
      locker.unlock();
    }
  }
  // this->pause_ = false;
  // this->stop_ = false;
  LOG("Exit Thread: " << std::this_thread::get_id());
}

void WThread::ProcessFunction() {
  LOG("Processing Function")
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
}
