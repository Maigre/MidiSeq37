#pragma once
#include "utils.h"
#include "conf.h"

int beats(int nBeat) {
  return nBeat*RESOLUTION;
}

int beat() {
  return RESOLUTION;
}
