#include <iostream>
#include <iomanip>
#include <gmpxx.h>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <future>

int n(0);
mpz_class pote(426880);
mpz_class sum(13591409);
mpq_class t(101);

std::condition_variable cond;
std::mutex m;
std::mutex m2;

void output() {
	auto out = [](int n, mpz_class pote, mpz_class sum, mpq_class t) -> mpq_class {
		if(n%2 == 0) {
			return mpq_class(pote*10005, sum)/t;
		} else {
			return mpq_class(pote, sum)*t;
		}
	};
	
	while (true) {
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
		
		std::cout << std::setprecision(192) << n1 << ": " << mpf_class(chudnovsky1, 1000) << std::endl;
		std::cout << std::setprecision(192) << n2 << ": " << mpf_class(chudnovsky2, 1000) << std::endl << std::endl;
	}
}

void chudnovsky() {
	mpz_class faku(1);
	
	while (true) {
		std::this_thread::sleep_for(std::chrono::nanoseconds(1));
		std::lock_guard<std::mutex> lck(m);
		//~ if (n%10000==0) std::cout << n << std::endl;
		
		auto a1 = std::async([&]() -> mpz_class {
			n++;
			faku *= (6_mpz*n-5)*(6_mpz*n-3)*(6_mpz*n-1)*8;
			faku /= mpz_class(n)*n*n;
			return (13591409+545140134_mpz*n)*faku;
		});
		
		auto a2 = std::async([&] {
			sum *= 262537412640768000_mpz;
		});
		
		auto a3 = std::async([&] {
			pote *= 262537412640768000_mpz;
		});
		
		a2.get();
		a3.get();
		
		if (n%2 == 0) {
			sum += a1.get();
		} else {
			sum -= a1.get();
		}
		
		cond.notify_all();
	}
}

void root() {
	while (true) {
		std::this_thread::sleep_for(std::chrono::nanoseconds(1));
		
		mpq_class te(t);
		te = (te*te+10005)/(te*2);
		
		std::lock_guard<std::mutex> lck2(m2);
		t = te;
	}
}

int main() {
	std::thread t1(output);
	std::thread t2(chudnovsky);
	std::thread t3(root);
	
	t1.join();
	t2.join();
	t3.join();

	return 0;
}
