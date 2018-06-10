// integral.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <ctime>
#include <chrono>
#include <omp.h>

#define partsPerUnit 300

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
		for (int i = 0; i < 100; i++) {
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
	m.integrateParallel1(0, 300, 1, 100); m.integrateParallel2(0, 300, 1, 100); m.integrateSerial(0, 300, 1, 100);
	m.checkEquality();
	lockScreen();

}