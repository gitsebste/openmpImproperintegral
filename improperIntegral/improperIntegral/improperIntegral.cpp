// integral.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <ctime>
#include <chrono>
#include <omp.h>

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
public:

	void integrateParallel1(double a, double b, int ki, int kf)
	{
		cout << "Integral I(k) z " << "f(x)=exp(-3 * x*pow(k, 0.5)) / (1 + x)" << " from a = " << a << " to b = " << b << endl;
		int numOfParts = (int)(b - a) * 3000;
		double dx = (b - a) / (numOfParts);

		double sum = 0;  int i = 0;
#pragma omp parallel for private(i) schedule(static) reduction(+:sum)
		for (int k = kf; k >= ki; k--)
		{
			waitSomeTime(1000);
			sum = 0;
			for (i = 0; i<numOfParts; i++)
			{
				sum += fun(a + i*dx + dx / 2, k);
			}
			sum *= dx;
#pragma omp critical
			cout << "for k = " << k << " I(" << k << ") = " << sum << endl;
		}
	}

	void integrateParallel2(double a, double b, int ki, int kf)
	{
		cout << "Integral I(k) z " << "f(x)=exp(-3 * x*pow(k, 0.5)) / (1 + x)" << " from a = " << a << " to b = " << b << endl;
		int numOfParts = (int)(b - a) * 3000;
		double dx = (b - a) / (numOfParts);

		double sum = 0;  int i = 0;
#pragma omp parallel for private(i) schedule(dynamic) reduction(+:sum)
		for (int k = kf; k >= ki; k--)
		{
			waitSomeTime(1000);
			sum = 0;
			for (i = 0; i<numOfParts; i++)
			{
				sum += fun(a + i*dx + dx / 2, k);
			}
			sum *= dx;
#pragma omp critical
			cout << "for k = " << k << " I(" << k << ") = " << sum << endl;
		}
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
	m.integrateParallel1(0, 300, 1, 100);
	lockScreen();

}