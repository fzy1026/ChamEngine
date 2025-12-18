#ifndef CHAMMATH_H
#define CHAMMATH_H

#include "chamtool.h"

class Matrix
{
    public:
    int hight,width;
    double **data;
    Matrix(int H,int W,double **Vals=NULL)
    {
        hight=H;
        width=W;
        data=new double *[hight];
        for(int i=0;i<hight;i++)
        {
            data[i]=new double [width];
            for(int j=0;j<width;j++)
            {
                if(Vals!=NULL && Vals[i]!=NULL)
                    data[i][j]=Vals[i][j];
                else
                    data[i][j]=0;
            }
        }
    }

    void SetZero()
    {
        for(int i=0;i<hight;i++)
        {
            for(int j=0;j<width;j++)
            {
                data[i][j]=0;
            }
        }
    }

    void SetIdentity()
    {
        SetZero();
        for(int i=0;i<min(hight,width);i++)
        {
            data[i][i]=1;
        }
    }

    bool Set(int x,int y,int val)
    {
        if(IsInRange(x,0,hight-1)&&IsInRange(y,0,width-1))
        {
            data[x][y]=val;
            return 1;
        }
        return 0;
    }

    double Get(int x,int y)
    {
        if(IsInRange(x,0,hight-1)&&IsInRange(y,0,width-1))
        {
            return data[x][y];
        }
        return 0;
    }

    Matrix operator+(const Matrix &a) const
    {
        if(hight!=a.hight||width!=a.width)
        {
            throw "Error: Matric size mismatch in addition.";
        }
        Matrix ans(hight,width);
        for(int i=0;i<hight;i++)
        {
            for(int j=0;j<width;j++)
            {
                ans.data[i][j]=data[i][j]+a.data[i][j];
            }
        }
        return ans;
    }

    Matrix operator*(const Matrix &a) const
    {
        if(width!=a.hight)
        {
            throw "Error: Matric size mismatch in multiplication.";
        }
        Matrix ans(hight,a.width);
        for(int i=0;i<hight;i++)
        {
            for(int j=0;j<a.width;j++)
            {
                for(int k=0;k<width;k++)
                {
                    ans.data[i][j]+=data[i][k]*a.data[k][j];
                }
            }
        }
        return ans;
    }

    Matrix operator*(const double &a) const
    {
        Matrix ans(hight,width);
        for(int i=0;i<hight;i++)
        {
            for(int j=0;j<width;j++)
            {
                ans.data[i][j]=data[i][j]*a;
            }
        }
        return ans;
    }

    Matrix operator*(const float &a) const
    {
        Matrix ans(hight,width);
        for(int i=0;i<hight;i++)
        {
            for(int j=0;j<width;j++)
            {
                ans.data[i][j]=data[i][j]*a;
            }
        }
        return ans;
    }

    Matrix operator*(const int &a) const
    {
        Matrix ans(hight,width);
        for(int i=0;i<hight;i++)
        {
            for(int j=0;j<width;j++)
            {
                ans.data[i][j]=data[i][j]*a;
            }
        }
        return ans;
    }

    Matrix operator*(const long &a) const
    {
        Matrix ans(hight,width);
        for(int i=0;i<hight;i++)
        {
            for(int j=0;j<width;j++)
            {
                ans.data[i][j]=data[i][j]*a;
            }
        }
        return ans;
    }

    Matrix operator*(const long long &a) const
    {
        Matrix ans(hight,width);
        for(int i=0;i<hight;i++)
        {
            for(int j=0;j<width;j++)
            {
                ans.data[i][j]=data[i][j]*a;
            }
        }
        return ans;
    }

    Matrix operator/(const double &a) const
    {
        Matrix ans(hight,width);
        for(int i=0;i<hight;i++)
        {
            for(int j=0;j<width;j++)
            {
                ans.data[i][j]=data[i][j]/a;
            }
        }
        return ans;
    }

    Matrix operator/(const float &a) const
    {
        Matrix ans(hight,width);
        for(int i=0;i<hight;i++)
        {
            for(int j=0;j<width;j++)
            {
                ans.data[i][j]=data[i][j]/a;
            }
        }
        return ans;
    }

    Matrix operator/(const int &a) const
    {
        Matrix ans(hight,width);
        for(int i=0;i<hight;i++)
        {
            for(int j=0;j<width;j++)
            {
                ans.data[i][j]=data[i][j]/a;
            }
        }
        return ans;
    }

    Matrix operator/(const long &a) const
    {
        Matrix ans(hight,width);
        for(int i=0;i<hight;i++)
        {
            for(int j=0;j<width;j++)
            {
                ans.data[i][j]=data[i][j]/a;
            }
        }
        return ans;
    }

    Matrix operator/(const long long &a) const
    {
        Matrix ans(hight,width);
        for(int i=0;i<hight;i++)
        {
            for(int j=0;j<width;j++)
            {
                ans.data[i][j]=data[i][j]/a;
            }
        }
        return ans;
    }

    Matrix SubMatrix(int x,int y)
    {
        if(hight<=1||width<=1||!IsInRange(x,0,hight-1)||!IsInRange(y,0,width-1))
        {
            throw "Error: Cannot get submatric.";
        }
        Matrix ans(hight-1,width-1);
        for(int i=0;i<hight;i++)
        {
            for(int j=0;j<width;j++)
            {
                if(i<x&&j<y)
                {
                    ans.data[i][j]=data[i][j];
                }
                else if(i<x&&j>y)
                {
                    ans.data[i][j-1]=data[i][j];
                }
                else if(i>x&&j<y)
                {
                    ans.data[i-1][j]=data[i][j];
                }
                else if(i>x&&j>y)
                {
                    ans.data[i-1][j-1]=data[i][j];
                }
                else
                {
                }
            }
        }
        return ans;
    }

    double Minor(int x,int y)
    {
        Matrix sub=SubMatrix(x,y);
        return sub.Det();
    }

    double Cofactor(int x,int y)
    {
        double minor=Minor(x,y);
        if((x+y)%2==0)
            return minor;
        else
            return -minor;
    }

    Matrix Transform()
    {
        Matrix ans(width,hight);
        for(int i=0;i<hight;i++)
        {
            for(int j=0;j<width;j++)
            {
                ans.data[j][i]=data[i][j];
            }
        }
        return ans;
    }

    double Det()
    {
        if(hight!=width)
        {
            throw "Error: Only square matric can calculate determinant.";
            return 0;
        }

        if(hight == 1)
        {
            return data[0][0];
        }
        else if(hight == 2)
        {
            return data[0][0]*data[1][1]-data[0][1]*data[1][0];
        }
        else
        {
            double ans = 0;
            for(int j=0;j<width;j++)
            {
                ans += data[0][j]*Cofactor(0,j);
            }
            return ans;
        }
    }

    Matrix Adj()
    {
        Matrix ans(hight,width);
        for(int i=0;i<hight;i++)
        {
            for(int j=0;j<width;j++)
            {
                ans.data[j][i]=Cofactor(i,j);
            }
        }
        return ans;
    }

    Matrix Inverse()
    {
        double determinant=Det();
        if(determinant==0)
        {
            throw "Error: Singular matric has no inverse matric.";
        }
        Matrix adj=Adj();
        return adj/determinant;
    }
};


bool MatrixAddable(const Matrix &a,const Matrix &b)
{
    return (a.hight==b.hight)&&(a.width==b.width);
}

bool MatrixMultipliable(const Matrix &a,const Matrix &b)
{
    return a.width==b.hight;
}


#endif // CHAMMATH_H