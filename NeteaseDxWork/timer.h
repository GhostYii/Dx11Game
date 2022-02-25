#pragma once
#include <chrono>

class Timer
{
public:
	Timer()
	{
		last = std::chrono::steady_clock::now();
	}
	// 返回从上次调用 Mark 所经过的时间
	float Mark()
	{
		const auto old = last;
		last = std::chrono::steady_clock::now();
		const std::chrono::duration<float> deltaTime = last - old;
		return deltaTime.count();
	}
	// 返回上次调用 Mark 调用的时间但不重置 Mark
	float Peek() const
	{
		return std::chrono::duration<float>(std::chrono::steady_clock::now() - last).count();
	}

private:
	std::chrono::steady_clock::time_point last;
};

