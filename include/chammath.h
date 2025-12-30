#ifndef CHAMMATH_H
#define CHAMMATH_H
#include <bits/stdc++.h>
#include <unistd.h>

const double PI = 3.1415926535897932384626;

class Matrix
{
    public:
    int hight,width;
    double **data;
    Matrix(int H,int W,double **Vals= nullptr);
    void SetZero();
    void SetIdentity();
    bool Set(int x,int y,int val);
    double Get(int x,int y);
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
    Matrix SubMatrix(int x,int y);
    double Minor(int x,int y);
    double Cofactor(int x,int y);
    Matrix Transform();
    double Det();
    Matrix Adj();
    Matrix Inverse();
};


bool MatrixAddable(const Matrix &a,const Matrix &b);
bool MatrixMultipliable(const Matrix &a,const Matrix &b);

int RAND();//生成范围为原版平方的大随机数


double Rand(double Min, double Max, double Div = 1);//生成[min,max]范围内，步长为Div的随机数


bool IsInRange(double x, double L, double R, bool HL = true, bool HR = true);//判断某数是否属于某区间

bool AngleIsInRange(double x, double L, double R, bool HL = true, bool HR = true);//判断某角度是否属于某角度区间

bool Equal(double a, double b, double eps = 1e-6);//浮点数判等

bool Equal(float a, float b, float eps = 1e-6f);

bool EqualZero(double a, double eps = 1e-6);//浮点数判0

bool EqualZero(float a, float eps = 1e-6f);


#endif // CHAMMATH_H