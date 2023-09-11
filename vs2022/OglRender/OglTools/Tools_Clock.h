#pragma once

#include <string>
#include <chrono>

namespace Tools
{
	class Tools_Clock
	{
	public:
		void Update();

		float GetFramerate();

		float GetDeltaTime();

		float GetDeltaTimeUnscaled();

		float GetTimeSinceStart();

		float GetTimeScale();

		void Scale(float p_coeff);

		void SetTimeScale(float p_timeScale);

	private:
		void Initialize();

		std::chrono::steady_clock::time_point	__START_TIME;
		std::chrono::steady_clock::time_point	__LAST_TIME;
		std::chrono::steady_clock::time_point	__CURRENT_TIME;
		std::chrono::duration<double>			__ELAPSED;

		bool	__INITIALIZED{ false };
		float	__TIME_SCALE{ 1.0f };
		float	__DELTA_TIME{ 0.0f };
		float	__TIME_SINCE_START{ 0.0f };
	};
}

