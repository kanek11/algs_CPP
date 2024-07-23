#pragma once
#include <chrono>

using namespace std::chrono;

class Timer
{
public:
    Timer() :
        m_start(steady_clock::now()),
        m_duration(duration<double>::zero())
    {
    }

    ~Timer()
    {
        stop();
    }

    void stop()
    {
        auto _end  = steady_clock::now();
        m_duration = _end - m_start;
    }

    template <typename DurationType = seconds>
    double getElapsedTime() const
    {
        return duration_cast<DurationType>(m_duration).count();
    }

private:
    time_point<steady_clock> m_start;

    duration<double> m_duration;
};