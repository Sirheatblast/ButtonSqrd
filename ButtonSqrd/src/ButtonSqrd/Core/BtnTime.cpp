#include"BtnTime.h"

namespace BtnSqd {
	float BtnTime::deltaTime = 0;
	std::chrono::steady_clock::time_point BtnTime::start = std::chrono::high_resolution_clock::now();
	std::chrono::steady_clock::time_point BtnTime::end = std::chrono::high_resolution_clock::now();
}