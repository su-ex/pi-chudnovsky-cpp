#include <iostream>
#include <iomanip>
#include <gmpxx.h>
#include <thread>
#include <condition_variable>
#include <mutex>

int n(0);
mpz_class pote(426880);
mpz_class sum(13591409);
mpq_class t(101);

std::condition_variable cond;
std::mutex m;
std::mutex m2;

void output() {
	while (true) {
		std::this_thread::sleep_for(std::chrono::nanoseconds(1));
		std::cout << "1" << std::endl;
		std::unique_lock<std::mutex> lck(m);
		std::cout << "2" << std::endl;
		cond.wait(lck);
		std::cout << "3" << std::endl;
		int n1(n);
		mpz_class sum1(sum);
		mpz_class pote1(pote);
		cond.wait(lck);
		std::cout << "3" << std::endl;
		int n2(n);
		mpz_class sum2(sum);
		mpz_class pote2(pote);
		lck.unlock();
		std::cout << "4" << std::endl;
		
		std::unique_lock<std::mutex> lck2(m2);
		std::cout << "5" << std::endl;
		//~ std::cout << std::setprecision(150) << mpf_class(t, 100000000) << std::endl;
		mpq_class ti(t);
		lck2.unlock();
		std::cout << "6" << std::endl;
		
		mpq_class outodd, outeven;
		if(n1%2 == 0) {
			outodd = mpq_class(pote1*10005, sum1)/ti;
			outeven = mpq_class(pote2, sum2)*ti;
		} else {
			outodd = mpq_class(pote2*10005, sum2)/ti;
			outeven = mpq_class(pote1, sum1)*ti;
		}
		
		//~ std::cout << std::setprecision(100000000) << ni << ": " << mpf_class(last, 100000000) << std::endl;
		//~ std::cout << "7" << std::endl;
		std::cout << n1 << ":" << n2 << std::endl;
	}
}

void chudnovsky() {
	mpz_class faku(1);
	mpz_class inte;
	
	while (true) {
		std::this_thread::sleep_for(std::chrono::nanoseconds(1));
		std::unique_lock<std::mutex> lck(m);
		//~ if (n%10000==0) std::cout << n << std::endl;
		
		//Thread 1.1:
		n++;
		faku *= (6*mpz_class(n)-5)*(6*n-3)*(6*n-1)*8;
		faku /= mpz_class(n)*n*n;
		inte = (13591409+545140134_mpz*n)*faku;
		//Thread 1.2:
		sum *= 262537412640768000_mpz;
		//Thread 1.3:
		pote *= 262537412640768000_mpz;
		
		if (n%2 == 0) {
			sum += inte;
		} else {
			sum -= inte;
		}
		
		cond.notify_all();
	}
}

void root() {
	while (true) {
		std::this_thread::sleep_for(std::chrono::nanoseconds(1));
		
		mpq_class te(t);
		te = (te*te+10005)/(te*2);
		
		std::unique_lock<std::mutex> lck2(m2);
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
