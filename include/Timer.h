#pragma once
#include <chrono>
#include <concepts>

namespace chr = std::chrono;

#define MEASURE_CALL_TIME(call, msg) \
{ auto startTime = chr::high_resolution_clock::now(); \
call \
auto endTime = chr::high_resolution_clock::now(); \
auto duration = chr::duration_cast<chr::milliseconds>(endTime - startTime); \
std::cout << "Call " << msg << " took " << duration.count() << " milliseconds" << std::endl; } \

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

    template<std::invocable _Fn, typename... Args>
    static auto MeasureDuration(_Fn f, Args&&... args)
    {
        auto startTime = chr::high_resolution_clock::now();
        f(std::forward<Args>(args)...);
        auto endTime = chr::high_resolution_clock::now();
        auto duration = chr::duration_cast<chr::milliseconds>(endTime - startTime);
        return duration.count();
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



