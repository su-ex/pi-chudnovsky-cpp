#include <iostream>
#include <iomanip>
#include <gmpxx.h>
#include <thread>
#include <condition_variable>
#include <mutex>

int n(1);
mpz_class pote(426880);
mpz_class sum(13591409);
mpq_class t(101);

std::condition_variable cond;
std::mutex m;
std::mutex m2;

void blabla() {
	mpq_class last;
	
	while (true) {
		std::cout << "1" << std::endl;
		std::unique_lock<std::mutex> lck(m);
		std::cout << "2" << std::endl;
		cond.wait(lck);
		std::cout << "3" << std::endl;
		int ni(n);
		mpz_class sumi(sum);
		mpz_class potei(pote);
		lck.unlock();
		std::cout << "4" << std::endl;
		
		std::unique_lock<std::mutex> lck2(m2);
		std::cout << "5" << std::endl;
		//~ std::cout << std::setprecision(150) << mpf_class(t, 100000000) << std::endl;
		mpq_class ti(t);
		lck2.unlock();
		std::cout << "6" << std::endl;
		
		if(ni%2!=0) {
			last = mpq_class(potei*10005, sumi)/ti;
		} else {
			last = mpq_class(potei, sumi)*ti;
		}
		
		std::cout << std::setprecision(150) << ni << ": " << mpf_class(last, 100000000) << std::endl;
		std::cout << "7" << std::endl;
	}
}

void pi() {
	mpz_class faku(1);
	mpz_class inte;
	
	while (true) {
		std::unique_lock<std::mutex> lck(m);
		//~ if (n%10000==0) std::cout << n << std::endl;
		
		//Thread 1.1:
		faku *= (6*mpz_class(n)-5)*(6*n-3)*(6*n-1)*8;
		faku /= mpz_class(n)*n*n;
		inte = (13591409+545140134_mpz*n++)*faku;
		//Thread 1.2:
		sum *= 262537412640768000_mpz;
		//Thread 1.3:
		pote *= 262537412640768000_mpz;
		
		if (n%2 != 0) {
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
		std::unique_lock<std::mutex> lck2(m2);
		t = (t*t+10005)/(t*2);
		lck2.unlock();
	}
}

int main() {

	std::thread t1(blabla);
	std::thread t2(pi);
	std::thread t3(root);
	
	t1.join();
	t2.join();
	t3.join();

	return 0;
}
