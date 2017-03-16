#include "Lockable.h"

//////////////////////////////////

void Lockable::lock() {
  mylock.lock();
}

void Lockable::unlock() {
  mylock.unlock();
}
