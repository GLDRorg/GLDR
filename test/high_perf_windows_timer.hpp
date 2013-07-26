#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <chrono>
#include <stdexcept>
#include <string>

// Self-made Windows QueryPerformanceCounter based C++11 API compatible clock
struct qpc_clock {
    typedef std::chrono::nanoseconds                       duration;      // nanoseconds resolution
    typedef duration::rep                                  rep;
    typedef duration::period                               period;
    typedef std::chrono::time_point<qpc_clock, duration>   time_point;
    static bool is_steady;                                                // = true
    static time_point now()
    {
        if (!is_inited) {
            init();
            is_inited = true;
        }
        LARGE_INTEGER counter;
        QueryPerformanceCounter(&counter);
        return time_point(duration(static_cast<rep>((double) counter.QuadPart / frequency.QuadPart *
            period::den / period::num)));
    }

private:
    static bool is_inited;                                                // = false
    static LARGE_INTEGER frequency;
    static void init()
    {
        if (QueryPerformanceFrequency(&frequency) == 0)
            throw std::logic_error("QueryPerformanceCounter not supported: " + std::to_string(GetLastError()));
    }
};

bool qpc_clock::is_steady = true;
bool qpc_clock::is_inited = false;
LARGE_INTEGER qpc_clock::frequency;

