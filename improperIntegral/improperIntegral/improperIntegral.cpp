// integral.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <ctime>
#include <chrono>
#include <omp.h>

#define partsPerUnit 2000
#define k_max 100

using namespace std;

void waitSomeTime(double t) {/*
							 chrono::steady_clock sc; // create an object of `steady_clock` class
							 auto start = sc.now(); // start timer
							 // #pragma omp parallel for
							 for (int i = 0;; i++) {
							 auto end = sc.now();
							 auto time_span = static_cast<chrono::duration<double>>(end - start); // measure time span between start & end
							 if (time_span.count() * 100000>t)break;

							 }
							 */
}

class My {
private:

	double para1[100]; double para2[100]; double seria[100]; 
public:
	void integrateParallel1(double a, double b, int ki, int kf)
	{
		//cout << "Integral I(k) z " << "f(x)=exp(-3 * x*pow(k, 0.5)) / (1 + x)" << " from a = " << a << " to b = " << b << endl;
		int numOfParts = (int)(b - a) * partsPerUnit;
		double dx = (b - a) / (numOfParts);

		double sum = 0;  int i = 0; int k;
#pragma omp parallel for private(k) schedule(static) reduction(+:sum)
		for (k = kf; k >= ki; k--)
		{
			sum = 0;
#pragma omp parallel for private(i) schedule(static) reduction(+:sum)
			for (i = 0; i<numOfParts; i++)
			{
				sum += fun(a + i*dx + dx / 2, k);
			}
			sum *= dx;
#pragma omp critical
			//cout << "for k = " << k << " I(" << k << ") = " << sum << endl;
			para1[k - 1] = sum;
		}
	}

	void integrateParallel2(double a, double b, int ki, int kf)
	{
		//cout << "Integral I(k) z " << "f(x)=exp(-3 * x*pow(k, 0.5)) / (1 + x)" << " from a = " << a << " to b = " << b << endl;
		int numOfParts = (int)(b - a) * partsPerUnit;
		double dx = (b - a) / (numOfParts);

		double sum = 0;  int i = 0; int k;
#pragma omp parallel for private(k) schedule(dynamic) reduction(+:sum)
		for (k = kf; k >= ki; k--)
		{
			sum = 0;
#pragma omp parallel for private(i) schedule(dynamic) reduction(+:sum)
			for (i = 0; i<numOfParts; i++)
			{
				sum += fun(a + i*dx + dx / 2, k);
			}
			sum *= dx;
#pragma omp critical
			para2[k - 1] = sum; ;// cout << "for k = " << k << " I(" << k << ") = " << sum << endl;
		}
	}
	void integrateSerial(double a, double b, int ki, int kf)
	{
		//cout << "Integral I(k) z " << "f(x)=exp(-3 * x*pow(k, 0.5)) / (1 + x)" << " from a = " << a << " to b = " << b << endl;
		int numOfParts = (int)(b - a) * partsPerUnit;
		double dx = (b - a) / (numOfParts);

		double sum = 0;  int i = 0;
		for (int k = kf; k >= ki; k--)
		{
			sum = 0;
			for (i = 0; i<numOfParts; i++)
			{
				sum += fun(a + i*dx + dx / 2, k);
			}
			sum *= dx;
			seria[k - 1] = sum; ;// cout << "for k = " << k << " I(" << k << ") = " << sum << endl;
		}
	}

	void checkEquality() {
		bool para1EqualsSerial = true; bool para2EqualsSerial = true;
		for (int i = 0; i < k_max; i++) {
			//cout << para1[i] << "\t" << seria[i] << "\t" << para2[i] << endl;
			printf("%10f\t%10f\t%10f\n", (float)para1[i], (float)seria[i], (float)para2[i]);
			//cout << para1[i] << endl;cout << seria[i]  << endl;cout << para2[i] << endl;
			if (para1[i] != seria[i])para1EqualsSerial = false;
			if (para2[i] != seria[i])para2EqualsSerial = false;
			if (para1EqualsSerial == false && para2EqualsSerial == false)break;
		}
		if (para1EqualsSerial)cout << "LEFT == MIDDLE"; else cout << "LEFT != MIDDLE"; cout << "\t\t";
		if (para2EqualsSerial)cout << "RIGHT == MIDDLE"; else cout << "RIGHT != MIDDLE"; cout << "\t";
	}


	double fun(double x, int k)
	{
		return exp(-3 * x*pow(k, 0.5)) / (1 + x);
	}

};

void lockScreen()
{
	char c;
	cin >> c;
}



int main() {
	My m;
	m.integrateParallel1(0, 232, 1, k_max); m.integrateParallel2(0, 400, 1, k_max); m.integrateSerial(0, 400, 1, k_max);
	m.checkEquality();
	lockScreen();

}
/*
k=1		0.262084	http://www.wolframalpha.com/input/?i=integral+(exp(-3+*+x*pow(1,+0.5))+%2F+(1+%2B+x))+from+0+to+inf
k=2		0.19628		http://www.wolframalpha.com/input/?i=integral+(exp(-3+*+x*pow(2,+0.5))+%2F+(1+%2B+x))+from+0+to+inf
k=3		0.164814	http://www.wolframalpha.com/input/?i=integral+(exp(-3+*+x*pow(3,+0.5))+%2F+(1+%2B+x))+from+0+to+inf
k=4		0.145268	http://www.wolframalpha.com/input/?i=integral+(exp(-3+*+x*pow(4,+0.5))+%2F+(1+%2B+x))+from+0+to+inf
k=5		0.131558	http://www.wolframalpha.com/input/?i=integral+(exp(-3+*+x*pow(5,+0.5))+%2F+(1+%2B+x))+from+0+to+inf
k=6		0.121234
.
.
.
k=100	0.0322897	http://www.wolframalpha.com/input/?i=integral+(exp(-3+*+x*pow(100,+0.5))+%2F+(1+%2B+x))+from+0+to+inf

OUTPUT:

0.262084        0.262084        0.262084
0.196280        0.196280        0.196280
0.164814        0.164814        0.164814
0.145268        0.145268        0.145268
0.131558        0.131558        0.131558
0.121234        0.121234        0.121234
0.113083        0.113083        0.113083
0.106431        0.106431        0.106431
0.100862        0.100862        0.100862
0.096109        0.096109        0.096109
0.091988        0.091988        0.091988
0.088370        0.088370        0.088370
0.085158        0.085158        0.085158
0.082281        0.082281        0.082281
0.079685        0.079685        0.079685
0.077326        0.077326        0.077326
0.075170        0.075170        0.075170
0.073188        0.073188        0.073188
0.071359        0.071359        0.071359
0.069664        0.069664        0.069664
0.068086        0.068086        0.068086
0.066614        0.066614        0.066614
0.065234        0.065234        0.065234
0.063939        0.063939        0.063939
0.062720        0.062720        0.062720
0.061569        0.061569        0.061569
0.060481        0.060481        0.060481
0.059449        0.059449        0.059449
0.058470        0.058470        0.058470
0.057538        0.057538        0.057538
0.056650        0.056650        0.056650
0.055803        0.055803        0.055803
0.054994        0.054994        0.054994
0.054219        0.054219        0.054219
0.053477        0.053477        0.053477
0.052765        0.052765        0.052765
0.052081        0.052081        0.052081
0.051423        0.051423        0.051423
0.050791        0.050791        0.050791
0.050181        0.050181        0.050181
0.049593        0.049593        0.049593
0.049026        0.049026        0.049026
0.048478        0.048478        0.048478
0.047948        0.047948        0.047948
0.047436        0.047436        0.047436
0.046940        0.046940        0.046940
0.046459        0.046459        0.046459
0.045993        0.045993        0.045993
0.045541        0.045541        0.045541
0.045102        0.045102        0.045102
0.044676        0.044676        0.044676
0.044262        0.044262        0.044262
0.043860        0.043860        0.043860
0.043468        0.043468        0.043468
0.043087        0.043087        0.043087
0.042716        0.042716        0.042716
0.042354        0.042354        0.042354
0.042002        0.042002        0.042002
0.041658        0.041658        0.041658
0.041323        0.041323        0.041323
0.040996        0.040996        0.040996
0.040676        0.040676        0.040676
0.040364        0.040364        0.040364
0.040059        0.040059        0.040059
0.039761        0.039761        0.039761
0.039470        0.039470        0.039470
0.039185        0.039185        0.039185
0.038907        0.038907        0.038907
0.038634        0.038634        0.038634
0.038367        0.038367        0.038367
0.038105        0.038105        0.038105
0.037849        0.037849        0.037849
0.037598        0.037598        0.037598
0.037352        0.037352        0.037352
0.037111        0.037111        0.037111
0.036874        0.036874        0.036874
0.036642        0.036642        0.036642
0.036415        0.036415        0.036415
0.036191        0.036191        0.036191
0.035972        0.035972        0.035972
0.035757        0.035757        0.035757
0.035545        0.035545        0.035545
0.035338        0.035338        0.035338
0.035134        0.035134        0.035134
0.034933        0.034933        0.034933
0.034736        0.034736        0.034736
0.034542        0.034542        0.034542
0.034352        0.034352        0.034352
0.034165        0.034165        0.034165
0.033980        0.033980        0.033980
0.033799        0.033799        0.033799
0.033621        0.033621        0.033621
0.033445        0.033445        0.033445
0.033272        0.033272        0.033272
0.033102        0.033102        0.033102
0.032935        0.032935        0.032935
0.032770        0.032770        0.032770
0.032607        0.032607        0.032607
0.032447        0.032447        0.032447
0.032289        0.032289        0.032289
LEFT == MIDDLE          RIGHT == MIDDLE

*/