#include <iostream>
#include <iomanip>
#include <gmpxx.h>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <future>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "TimeMeasurement.h"

int timesAtNsIteration = 1;

int n(0);
mpz_class pote(426880);
mpz_class sum(13591409);
mpq_class t(101);

std::condition_variable cond;
std::mutex m;
std::mutex m2;
std::mutex moutput;

void output() {
	auto out = [](int n, mpz_class pote, mpz_class sum, mpq_class t) -> mpq_class {
		if(n%2 == 0) {
			return mpq_class(pote*10005, sum)/t;
		} else {
			return mpq_class(pote, sum)*t;
		}
	};
	
	while (n < 800000) {
		std::this_thread::sleep_for(std::chrono::nanoseconds(1));
		
		std::unique_lock<std::mutex> lck2(m2);
		mpq_class ti(t);
		lck2.unlock();
		
		std::unique_lock<std::mutex> lck(m);
		cond.wait(lck);
		int n1(n);
		auto a1 = std::async(out, n, pote, sum, ti);
		cond.wait(lck);
		int n2(n);
		auto a2 = std::async(out, n, pote, sum, std::move(ti));
		lck.unlock();
		
		mpq_class chudnovsky1 = a1.get();
		mpq_class chudnovsky2 = a2.get();
		
		std::unique_lock<std::mutex> olck(moutput);
		std::cout << std::setprecision(192) << n1 << ": " << mpf_class(chudnovsky1, 1000) << std::endl;
		std::cout << std::setprecision(192) << n2 << ": " << mpf_class(chudnovsky2, 1000) << std::endl << std::endl;
		olck.unlock();
	}
}

void chudnovsky() {
	mpz_class faku(1);
	
	while (n < 800000) {
		std::this_thread::sleep_for(std::chrono::nanoseconds(1));
		std::lock_guard<std::mutex> lck(m);
		
		std::unique_lock<std::mutex> olck(moutput);
		if (n % timesAtNsIteration == 0) std::cout << "n: " << n << "\t";
		olck.unlock();
		
		auto a1 = std::async([&]() -> mpz_class {
			TimeMeasurement tm("a1");
			n++;
			faku *= (6_mpz*n-5)*(6_mpz*n-3)*(6_mpz*n-1)*8;
			faku /= mpz_class(n)*n*n;
			return (13591409+545140134_mpz*n)*faku;
		});
		
		auto a2 = std::async([&] {
			TimeMeasurement tm("a2");
			sum *= 262537412640768000_mpz;
		});
		
		auto a3 = std::async([&] {
			TimeMeasurement tm("a3");
			pote *= 262537412640768000_mpz;
		});
		
		a2.get();
		a3.get();
		
		{
			TimeMeasurement tm("add");
			if (n%2 == 0) {
				sum += a1.get();
			} else {
				sum -= a1.get();
			}
		}
		
		olck.lock();
		if (n % timesAtNsIteration == 0) std::cout << std::endl;
		olck.unlock();
		
		cond.notify_all();
	}
}

void root() {
	while (n < 800000) {
		std::this_thread::sleep_for(std::chrono::nanoseconds(1));
		
		mpq_class te(t);
		te = (te*te+10005)/(te*2);
		
		std::lock_guard<std::mutex> lck2(m2);
		t = te;
	}
}

TimeMeasurement tm("\ntotal");

void my_handler(int s) {
	exit(1); 

}

int main() {
	struct sigaction sigIntHandler;
	
	sigIntHandler.sa_handler = my_handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	
	sigaction(SIGINT, &sigIntHandler, NULL);
	
	//~ std::thread t1(output);
	std::thread t2(chudnovsky);
	//~ std::thread t3(root);
	
	//~ t1.join();
	t2.join();
	//~ t3.join();

	return 0;
}
