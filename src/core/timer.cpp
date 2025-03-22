#include "timer.h"

Timer::Timer() {
  start();
}

void Timer::start() {
  t1 = std::chrono::high_resolution_clock::now();
}

double Timer::stop() {
  t2 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = t2 - t1;
  return elapsed.count();
}