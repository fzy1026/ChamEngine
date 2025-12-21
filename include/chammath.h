#ifndef CHAMMATH_H
#define CHAMMATH_H

#include "chamtool.h"

class Matrix
{
    public:
    int hight,width;
    double **data;
    Matrix(int H,int W,double **Vals=NULL);
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


#endif // CHAMMATH_H