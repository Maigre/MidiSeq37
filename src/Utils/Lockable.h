#pragma once
#include <mutex>

class Lockable {
  protected:
    void lock();
    void unlock();
  private:
    std::mutex mylock;
};
