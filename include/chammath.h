#ifndef CHAMMATH_H
#define CHAMMATH_H
#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

const double PI = 3.1415926535897932384626;
class Matrix
{
    public:
    int hight,width;
    vector<vector<double>> data;
    Matrix(int H,int W);
    void SetZero();//全0矩阵，初始化用
    void SetIdentity();//变为但未矩阵
    bool Set(int x,int y,double val);//更改单个元素值 
    double Get(int x,int y);//获取单个元素值
    Matrix operator+(const Matrix &a) const;
    Matrix operator*(const Matrix &a) const;
    Matrix operator*(const double &a) const;
    Matrix operator*(const float &a) const;
    Matrix operator*(const int &a) const;
    Matrix operator*(const long &a) const;
    Matrix operator*(const long long &a) const;
    Matrix operator/(const double &a) const;
    Matrix operator/(const float &a) const;
    Matrix operator/(const int &a) const;
    Matrix operator/(const long &a) const;
    Matrix operator/(const long long &a) const;

    Matrix SubMatrix(int x,int y);//余子式矩阵
    double Minor(int x,int y);//余子式
    double Cofactor(int x,int y);//代数余子式
    Matrix Transform();//转置矩阵
    double Det();//行列式
    Matrix Adj();//伴随矩阵
    Matrix Inverse();//逆矩阵
};




bool MatrixAddable(const Matrix &a,const Matrix &b);//检测两个矩阵是否可相加
bool MatrixMultipliable(const Matrix &a,const Matrix &b);//检测两个矩阵是否可相乘

int RAND();//生成范围为原版平方的大随机数


double Rand(double Min, double Max, double Div = 1);//生成[min,max]范围内，步长为Div的随机数


bool InRange(double x, double L, double R, bool HL = true, bool HR = true);//判断某数是否属于某区间,

bool AngleIsInRange(double x, double L, double R, bool HL = true, bool HR = true);//判断某角度是否属于某角度区间

bool Equal(double a, double b, double eps = 1e-6);//浮点数判等

bool Equal(float a, float b, float eps = 1e-6f);

bool EqualZero(double a, double eps = 1e-6);//浮点数判0

bool EqualZero(float a, float eps = 1e-6f);

bool AngleIntersects(double AStartAng,double AEndAng,double BStartAng,double BEndAng);//检测两个角是否有重合部分


#endif // CHAMMATH_H