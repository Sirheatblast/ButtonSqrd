#pragma once
#include<chrono>

namespace BtnSqd {
	class BtnTime {
	public:
		static float GetDeltaTime() {
			return deltaTime;
		}

		static void CalculateDeltaTimeStart() {
			start = std::chrono::high_resolution_clock::now();
		}

		static void CalculateDeltaTimeEnd() {
			end = std::chrono::high_resolution_clock::now();

			std::chrono::duration<float> delta = end - start;
			deltaTime = delta.count();
		}

	private:
		static std::chrono::steady_clock::time_point start, end;
		static float deltaTime;
	};
}