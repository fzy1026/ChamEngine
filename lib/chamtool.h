#ifndef CHAMTOOL_H
#define CHAMTOOL_H
#include <easyx/easyx.h>
#include <easyx/graphics.h>
#include <math.h>
#include <unistd.h>
#include <conio.h>
#include <windows.h>
#include <string>
#include <vector>
#include <iostream>


using namespace std;
const double PI = 3.14159;

vector<int> TestVector;

//生成范围为原版平方的大随机数
int RAND()
{
	return rand() * RAND_MAX + rand();
}


//生成[min,max]范围内，步长为Div的随机数
double Rand(double Min, double Max, double Div = 1)
{
	int nmin = floor(Min / Div);
	int nmax = floor(Max / Div);
	return double(RAND() % (nmax - nmin + 1) + nmin) * Div;
}


bool IsInRange(double x, double L, double R, bool HL = true, bool HR = true)
{
	if (L > R)
	{
		swap(L, R);
		swap(HL, HR);
	}

	bool ans = 1;

	if (HL)
		ans = ans && x >= L;
	else
		ans = ans && x > L;

	if (HR)
		ans = ans && x <= R;
	else
		ans = ans && x < R;

	return ans;
}

bool AngleIsInRange(double x, double L, double R, bool HL = true, bool HR = true)
{
	while(R < L)
		R += 2 * PI;
	while(R - L > 2*PI)
		R -= 2 * PI;
	while (x < L)
		x += 2 * PI;
	while(x - 2*PI > L)
		x -= 2 * PI;
	bool ans = 1;
	if(HL)
		ans = ans && x >= L;
	else
		ans = ans && x > L;
	if(HR)
		ans = ans && x <= R;
	else
		ans = ans && x < R;

	return ans;
}

bool Equal(double a, double b, double eps = 1e-6)
{
	return fabs(a - b) < eps;
}

bool Equal(float a, float b, float eps = 1e-6f)
{
	return fabs(a - b) < eps;
}

bool EqualZero(double a, double eps = 1e-6)
{
	return fabs(a) < eps;
}

bool EqualZero(float a, float eps = 1e-6f)
{
	return fabs(a) < eps;
}




#endif
