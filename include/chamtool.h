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


//生成范围为原版平方的大随机数
int RAND();


//生成[min,max]范围内，步长为Div的随机数
double Rand(double Min, double Max, double Div = 1);


bool IsInRange(double x, double L, double R, bool HL = true, bool HR = true);

bool AngleIsInRange(double x, double L, double R, bool HL = true, bool HR = true);

bool Equal(double a, double b, double eps = 1e-6);

bool Equal(float a, float b, float eps = 1e-6f);

bool EqualZero(double a, double eps = 1e-6);

bool EqualZero(float a, float eps = 1e-6f);




#endif
