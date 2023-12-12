#pragma once
#include <chrono>

namespace timer
{
	using Clock = std::chrono::high_resolution_clock;
	using Timestamp = std::chrono::steady_clock::time_point;
	using Duration = std::chrono::steady_clock::duration;
	using Seconds = std::chrono::seconds;
	using Milliseconds = std::chrono::milliseconds;

	constexpr inline Seconds to_seconds(Duration dur)
	{
		return std::chrono::duration_cast<Seconds>(dur);
	}

	constexpr inline Milliseconds to_milliseconds(Duration dur)
	{
		return std::chrono::duration_cast<Milliseconds>(dur);
	}

	class Timer
	{
		Timestamp m_start;

	public:
		Timer()
		{
			start();
		}

		inline void start()
		{
			m_start = Clock::now();
		}
		
		inline Duration now()
		{
			return Clock::now() - m_start;
		}

		Duration restart()
		{
			Duration dur = Clock::now() - m_start;
			m_start = Clock::now();
			return dur;
		}
	};

}