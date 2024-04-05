#pragma once
#include <chrono>

using namespace std::chrono;

class FrameTime
{

public:
	FrameTime() noexcept;

	float Tick() noexcept;

	float TotalTime() const noexcept;

	void Pause() noexcept;

	void Resume() noexcept;

	void Reset() noexcept;

private:
	time_point<steady_clock> m_startTime;
	time_point<steady_clock> m_lastTime;
	time_point<steady_clock> m_stopTime;
	duration<float> m_pausedTime;
	bool m_stopped;
};

