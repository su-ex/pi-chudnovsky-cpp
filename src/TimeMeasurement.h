#ifndef TIMEMEASUREMENT_H
#define TIMEMEASUREMENT_H

#include <string>
#include <chrono>
#include <mutex>

extern std::mutex moutput;

class TimeMeasurement {
private:
	std::chrono::high_resolution_clock::time_point start;
	std::chrono::high_resolution_clock::time_point end;
	std::string label;
public:
	TimeMeasurement(const std::string);
	~TimeMeasurement();
};

#endif
