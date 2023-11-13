#pragma once
#include <chrono>

class Timer
{
public:
    Timer() : tp(std::chrono::system_clock::now())
    {

    }
    // some kind of float
    static float Peek()
    {
        using namespace std::chrono;
        return (time_point_cast<milliseconds>(system_clock::now()) - time_point_cast<hours>(system_clock::now())).count() / 1000.f;
    }

    // Updates timer and restarts counting
    float Check()
    {
        using namespace std::chrono;
        auto time_passed = system_clock::now() - tp;
        tp = system_clock::now();
        return duration<float>(time_passed).count();
    }

    std::chrono::system_clock::time_point tp;
};



