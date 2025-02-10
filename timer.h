#pragma once

#include <chrono>

class Timer {
public:
    Timer();
    void start();
    double stop();
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> t1, t2;
};
