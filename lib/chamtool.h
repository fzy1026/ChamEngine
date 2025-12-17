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

int RAND()
{
	return rand() * RAND_MAX + rand();
}
double Rand(double Min, double Max, double Div = 1)
{
	int nmin = floor(Min / Div);
	int nmax = floor(Max / Div);
	return double(RAND() % (nmax - nmin + 1) + nmin) * Div;
}
bool LastKeyState[256];//128key's State
void KeyRefresh(int c)
{
	LastKeyState[c] = GetKeyState(c) < 0;
}
bool KeyDown(int c)
{
	/*
	BYTE keys[256];
	if (!GetKeyboardState(keys))
	{
		cout<<"Error:读取键盘失败\n";
		return 0;
	}
	bool ans = keys[c] & 0x80;
	cout<<"ans = "<<keys[c]<<endl;
	*/
	bool ans = GetKeyState(c) < 0;
	KeyRefresh(c);

	if (ans)
	{
		return 1;
	}

	return 0;
}
bool KeyClick(int c)
{
	bool ans = GetKeyState(c) < 0;
	bool Last = LastKeyState[c];
	KeyRefresh(c);

	if (ans && !Last)
	{
		return 1;
	}

	return 0;
}
bool Equal(double a, double b)
{
	return abs(a - b) <= 0.1;
}
bool IsANum(string s)
{
	bool P = 0;

	for (int i = 0; i < int(s.length()); i++)
	{
		if (i == 0 && s[i] == '-')
		{
			continue;
		}

		if (i == 0 && s[i] == '.')
		{
			return 0;
		}

		if (s[i] == '.' && !P)
		{
			P = 1;
			continue;
		}

		if (s[i] == '.' && P)
		{
			return 0;
		}

		if (s[i] < '0' || s[i] > '9')
		{
			return 0;
		}
	}

	return 1;
}
double StoD(string s)//string to double
{
	double z = 0;
	double x = 0.0;
	int xNum = 1;
	bool Now = 1;
	double SBZTY = 1.0;

	for (int i = 0; i < int(s.length()); i++)
	{
		if (s[i] == '-')
		{
			SBZTY = -1.0;
			continue;
		}

		if (s[i] == '.')
		{
			Now = 0;
			continue;
		}

		if (Now)
		{
			z *= 10;
			z += int(s[i] - '0');
		}
		else
		{
			x += pow(0.1, xNum) * double(s[i] - '0');
			xNum++;
		}
	}

	return SBZTY * (z + x);
}
string DtoS(double d)//double to string
{
	int D;
	string ans = "";
	D = d;//int section;
	d -= D;//<0 section;

	while (D > 0)
	{
		ans = char(D % 10 + '0') + ans;
		D /= 10;
	}

	if (Equal(d, 0))
	{
		return ans;
	}
	else
	{
		ans = ans + '.';

		while (!Equal(d, 0))
		{
			d *= 10;
			D = d;
			ans = ans + char(D % 10 + '0');
			d -= D;
		}
	}

	return ans;
}
string AllUpper(string s)
{
	for (int i = 0; i < int(s.length()); i++)
	{
		if (s[i] >= 'a' && s[i] <= 'z')
		{
			s[i] += ('A' - 'a');
		}
	}

	return s;
}
string AllLower(string s)
{
	for (int i = 0; i < int(s.length()); i++)
	{
		if (s[i] >= 'A' && s[i] <= 'Z')
		{
			s[i] -= ('A' - 'a');
		}
	}

	return s;
}
string StringCut(string s, int l, int r) 
{
	string ans = "";

	for (int i = l; i <= r; i++)
	{
		ans += s[i];
	}

	return ans;
}
string StringDel(string s, int l, int r) //�ַ���ɾ��
{
	string ans = "";

	for (int i = 0; i < l; i++)
	{
		ans += s[i];
	}

	for (int i = r + 1; i < int(s.length()); i++)
	{
		ans += s[i];
	}

	return ans;
}
bool Belong(double x, double L, double R, bool HL = true, bool HR = true) //�ж���������
{
	if (L > R) //��ֹ������
	{
		swap(L, R);
		swap(HL, HR);
	}

	bool ans = 1;

	if (HL)
	{
		ans = ans && x >= L;
	}
	else
	{
		ans = ans && x > L;
	}

	if (HR)
	{
		ans = ans && x <= R;
	}
	else
	{
		ans = ans && x < R;
	}

	return ans;
}

bool AngleBelong(double AngleX, double L, double R, bool HL = true, bool HR = true)
{
	return Belong(AngleX, L, R, HL, HR) || Belong(AngleX + PI * 2, L, R, HL, HR) || Belong(AngleX - PI * 2, L, R, HL, HR);
}

vector<double> Equations(double A1, double B1, double C1, double A2, double B2, double C2)
{
	vector<double> ans;
	ans.clear();
	double delta = A1 * B2 - A2 * B1;

	if (Equal(delta, 0))
	{
		return ans;
	}
	else
	{
		ans.push_back((B2 * C1 - B1 * C2) / (A2 * B1 - A1 * B2));
		ans.push_back((A2 * C1 - A1 * C2) / (B2 * A1 - B1 * A2));
		return ans;
	}
}
vector<double> Equation2(double a, double b, double c) //ax^2+bx+c = 0
{
	vector<double> ans;
	ans.clear();
	double delta = b * b - 4.0 * a * c;

	if (Equal(delta, 0))
	{
		ans.push_back(-b / (2.0 * a));
	}
	else if (delta < 0)
	{
		return ans;
	}
	else
	{
		delta = sqrt(delta);
		ans.push_back((-b + delta) / (2.0 * a));
		ans.push_back((-b - delta) / (2.0 * a));
	}

	return ans;
}
double CosA(double a, double b, double c)
{
	return double((b * b + c * c - a * a) / (2.0 * b * c));
}
double Rebound(double movea, double walla)
{
	double da = movea - walla;
	return walla - da;
}
#endif
