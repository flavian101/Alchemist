#include "FrameTime.h"

FrameTime::FrameTime() noexcept
	: m_startTime(steady_clock::now()),
	m_lastTime(m_startTime),
	m_pausedTime(0),
	m_stopped(false) {}



// Call this every frame
float FrameTime::Tick() noexcept
{
    if (m_stopped)
    {
        return 0.0f;
    }

    auto currentTime = steady_clock::now();
    const duration<float> frameTime = currentTime - m_lastTime;
    m_lastTime = currentTime;
    return frameTime.count();
}

// Call this to get total time since start (excluding paused time)
float FrameTime::TotalTime() const noexcept
{
    if (m_stopped)
    {
        return duration<float>(m_stopTime - m_startTime - m_pausedTime).count();
    }
    else
    {
        return duration<float>(steady_clock::now() - m_startTime - m_pausedTime).count();
    }
}

// Call this to pause the timer
void FrameTime::Pause() noexcept
{
    if (!m_stopped)
    {
        m_stopTime = steady_clock::now();
        m_stopped = true;
    }
}

// Call this to resume the timer
void FrameTime::Resume() noexcept
{
    if (m_stopped)
    {
        auto startTime = steady_clock::now();
        m_pausedTime += startTime - m_stopTime;
        m_lastTime = startTime;
        m_stopped = false;
    }
}

// Call this to reset the timer
void FrameTime::Reset() noexcept
{
    auto currentTime = steady_clock::now();
    m_startTime = currentTime;
    m_lastTime = currentTime;
    m_pausedTime = duration<float>::zero();
    m_stopped = false;
}