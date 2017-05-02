#include "TimeMeasurement.h"
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <future>

TimeMeasurement::TimeMeasurement(const std::string label) : label(label) {
	start = std::chrono::high_resolution_clock::now();
}

TimeMeasurement::~TimeMeasurement() {
	end = std::chrono::high_resolution_clock::now();
	std::async([&] {
		std::lock_guard<std::mutex> lck(moutput);
		std::cout << label << ": " << std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count() << " ns" << "\t";
	});
}
