#include <Nova/Time.hpp>

#include <chrono>

std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
std::chrono::duration<int, std::milli> deltaTime;

float Time::DeltaTime()
{
	return deltaTime.count() / 1000.0f;
}
