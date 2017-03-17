#include <iostream>
#include <iomanip>
#include <gmpxx.h>

int main() {
	int n(1);
	mpz_class faku(1);
	mpz_class pote(426880);
	mpz_class inte;
	mpz_class sum(13591409);
	mpq_class t(1);

	while (true) {
		//Thread 0:
		std::cout << std::setprecision(1000) << n << ": " << mpf_class(mpq_class(pote, sum)*t, 100000000) << std::endl;

		//Thread 1:
			//Thread 1.1:
			faku *= (6*n-5)*(6*n-3)*(6*n-1)*8;
			faku /= n*n*n;
			inte = (13591409+545140134_mpz*n++)*faku;
			//Thread 1.2:
			sum *= 262537412640768000_mpz;
			//Thread 1.3:
			pote *= 262537412640768000_mpz;
		if(n%2!=0){
			sum += inte;
		}
		else{
			sum -= inte;
		}

		//Thread 2:
		t = (t*t+10005)/(t*2);
	}

	return 0;
}
