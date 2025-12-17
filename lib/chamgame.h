#ifndef CHAMGAME_H
#define CHAMGAME_H
#include "chamtool.h"
using namespace std;
double WinHeight, WinWidth;
int FPS = 60;
void SetFPS(int f)
{
	FPS = f;
}
void EngineInit(int width, int height)
{
	WinHeight = height;
	WinWidth = width;
	initgraph(width, height);
}
void EngineInit(int width, int height, int flag)
{
	WinHeight = height;
	WinWidth = width;
	initgraph(width, height, flag);
}
void ClearScreen()
{
	clearrectangle(0, 0, WinWidth, WinHeight);
}

class Point {
	public:
		double x, y;
		COLORREF color;
		void Debug()
		{
			cout << "(" << x << "," << y << ")\n";
		}
		Point(double X = 0, double Y = 0, COLORREF Color = WHITE)
		{
			x = X;
			y = Y;
			color = Color;
		}
		Point(double X, double Y, int R, int G, int B)
		{
			x = X;
			y = Y;
			color = RGB(R, G, B);
		}
		void Draw()
		{
			setfillcolor(color);
			fillcircle(x, y, 2);
			//putpixel(x,y,color);
		}
	
		Point Move(double dx, double dy)
		{
			return Point(x + dx, y + dy, color);
		}
		bool operator==(const Point b)
		{
			return Equal(x, b.x) && Equal(y, b.y);
		}
};

class Image{
public:
	Point pos;//the postion of the image's Left-Up point
	IMAGE file;
	LPCTSTR address;
	
	
	void Move(double dx,double dy)
	{
		pos.Move(dx,dy);
	}
	
	void SetAddress(string s)
	{
		//address = s.c_str();
	}
	
	void LoadFile(int Nw = 0,int Nh = 0)
	{
		//loadimage(&file,address,Nw,Nh);
	}
	
	void Draw()
	{
		putimage(pos.x,pos.y,&file);
	}
	
	Image(int X = 0,int Y = 0,string Address = "",int Nw = 0,int Nh = 0)
	{
		pos.x = X;
		pos.y = Y;
		SetAddress(Address);
		LoadFile(Nw,Nh);
	}
};

Point Polor(double Ang, double r, Point center)
{
	center.x += cos(Ang) * r;
	center.y -= sin(Ang) * r;
	return center;
}
Point Middle(Point p1, Point p2)
{
	return Point((p1.x + p2.x) / 2.0, (p1.y + p2.y) / 2.0);
}
double Distance(Point a, Point b)
{
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}
double Angle(Point start, Point end)
{
	double ans = 0;

	if (start == end)
	{
		return 0;
	}

	if (Equal(start.x, end.x))
	{
		if (start.y > end.y)
		{
			return PI / 2.0;
		}
		else
		{
			return 1.5 * PI;
		}
	}

	ans = -atan((end.y - start.y) / (end.x - start.x));

	if (end.x < start.x)
	{
		ans += PI;
	}

	while (ans < 0)
	{
		ans += PI * 2;
	}

	return ans;
}
Point Zoom(Point P, Point c, double Size)
{
	double dis = Distance(P, c);
	double Ang = Angle(c, P);
	dis *= Size;
	return Polor(Ang, dis, c);
}
class Line {
	public:
		Point p[2];
		bool is_arc;
		Point o;
		double a, b, c; //ay+bx+c = 0;
		double starta, enda, r;
		COLORREF color;
		void Debug()
		{
			cout << "Line Debug:" << endl;
			cout << "p[0].debug:" << endl;
			p[0].Debug();
			cout << "p[1].debug:" << endl;
			p[1].Debug();
			cout << "o.debug:" << endl;
			o.Debug();
			cout << "is_arc=" << is_arc << " a=" << a << " b=" << b << " c=" << c << endl;
			cout << "starta=" << starta << " enda=" << enda << " r=" << r << endl;
			cout << "color=" << color << endl;
			cout << "Line debug end\n";
		}
		Line()
		{
			is_arc = 0;
			color = WHITE;
			p[0] = Point(0, 0);
			p[1] = Point(0, 0);
			a = 0;
			b = 0;
			c = 0;
			starta = 0;
			enda = 0;
			r = 0;
			o = Point(0, 0);
		}
		Line(Point p1, Point p2, COLORREF Color = WHITE)
		{
			is_arc = 0;
			color = Color;
			p[0] = p1;
			p[1] = p2;

			if (p1.x == p2.x)
			{
				a = 0;
				c = 0 - p1.x;
				b = 1;
			}
			else if (p1.y == p2.y)
			{
				b = 0;
				a = 1;
				c = 0 - p1.y;
			}
			else
			{
				a = 1;
				b = (p2.y - p1.y) / (p1.x - p2.x);
				c = (p1.y * p2.x - p2.y * p1.x) / (p1.x - p2.x);
			}
		}
		Line(Point StartP, Point EndP, Point O, COLORREF Color = WHITE)
		{
			a = 0;
			b = 0;
			c = 0;
			is_arc = 1;
			o = O;
			p[0] = StartP;
			p[1] = EndP;
			color = Color;
			r = Distance(p[0], o);
			starta = Angle(o, p[0]);
			enda = Angle(o, p[1]);

			if (enda <= starta)
			{
				enda += 2 * PI;
			}

			while (enda < 0 && starta < 0)
			{
				enda += 2 * PI;
				starta += 2 * PI;
			}

			while (enda > 2 * PI && starta > 2 * PI)
			{
				enda -= 2 * PI;
				starta -= 2 * PI;
			}
		}
		Line(Point O, double StartAngle, double EndAngle, double R, COLORREF Color = WHITE)
		{
			a = 0;
			b = 0;
			c = 0;
			o = O;
			starta = StartAngle;
			enda = EndAngle;
			color = Color;
			is_arc = 1;
			r = R;
			p[0] = Polor(StartAngle, r, O);
			p[1] = Polor(EndAngle, r, O);

			if (enda <= starta)
			{
				enda += 2 * PI;
			}

			while (enda < 0 && starta < 0)
			{
				enda += 2 * PI;
				starta += 2 * PI;
			}

			while (enda > 2 * PI && starta > 2 * PI)
			{
				enda -= 2 * PI;
				starta -= 2 * PI;
			}
		}
		void Draw()
		{
			setlinecolor(color);

			if (is_arc)
			{
				arc(o.x - r, o.y + r, o.x + r, o.y - r, starta, enda);
			}
			else
			{
				line(p[0].x, p[0].y, p[1].x, p[1].y);
			}
		}
		bool PointBelong(Point x)
		{
			if (!is_arc)
			{
				if ((Belong(x.x, p[0].x, p[1].x) && Belong(x.y, p[0].y, p[1].y)) && Equal(a * x.y + b * x.x + c, 0))
				{
					return 1;
				}

				return 0;
			}
			else
			{
				double pa = Angle(o, x);

				if (!Equal(Distance(x, o), r))
				{
					return 0;
				}
				else
				{
					if (pa > enda)
					{
						while (pa > enda)
						{
							pa -= PI * 2;
						}
					}
					else if (pa < starta)
					{
						while (pa < starta)
						{
							pa += PI * 2;
						}
					}
					else
					{
					}

					return Belong(pa, starta, enda);
				}
			}
		}
};
vector<Point> Intersection(Line l1, Line l2)
{
	vector<Point> ans;
	ans.clear();

	if (l1.is_arc == 0 && l2.is_arc == 0)
	{
		//ay+bx+c
		vector<double> p_ans = Equations(l1.b, l1.a, l1.c, l2.b, l2.a, l2.c);

		if (p_ans.empty())
		{
			return ans;
		}
		else
		{
			Point P(p_ans[0], p_ans[1]);

			if (l1.PointBelong(P) && l2.PointBelong(P))
			{
				ans.push_back(P);
			}
		}
	}
	else if (l1.is_arc && l2.is_arc)
	{
		//	double x1 = l1.o.x, y1 = l1.o.y;
		double r1 = l1.r;
		//	double x2 = l2.o.x, y2 = l2.o.y;
		double r2 = l2.r;
		double Ang = Angle(l1.o, l2.o);
		double dis = Distance(l1.o, l2.o);
		double Ang2 = acos(CosA(r2, r1, dis));
		Point ans1 = Polor(Ang + Ang2, r1, l1.o);
		Point ans2 = Polor(Ang - Ang2, r1, l1.o);

		if (l1.PointBelong(ans1) && l2.PointBelong(ans1))
		{
			ans.push_back(ans1);
		}

		if (l1.PointBelong(ans2) && l2.PointBelong(ans2))
		{
			ans.push_back(ans2);
		}
	}
	else
	{
		if (l2.is_arc)
			swap(l1, l2);

		double x0 = l1.o.x, y0 = l1.o.y, R = l1.r, A = l2.a, B = l2.b, C = l2.c;

		if (Equal(A, 0))   //Bx+C = 0
		{
			double X = -C / B;

			if (pow(X - x0, 2) > pow(R, 2)) //no ans
			{
				return ans;
			}
			else
			{
				double Y1 = y0 + sqrt(R * R - pow(X - x0, 2));
				double Y2 = y0 - sqrt(R * R - pow(X - x0, 2));

				if (l1.PointBelong(Point(X, Y1)) && l2.PointBelong(Point(X, Y1)))
				{
					ans.push_back(Point(X, Y1));
				}

				if (l1.PointBelong(Point(X, Y2)) && l2.PointBelong(Point(X, Y1)))
				{
					ans.push_back(Point(X, Y2));
				}
			}
		}
		else if (Equal(B, 0))     //Ay+c = 0
		{
			double Y = -C / A;

			if (pow(Y - y0, 2) > pow(R, 2)) //no ans
			{
				return ans;
			}
			else
			{
				double X1 = x0 + sqrt(R * R - pow(Y - y0, 2));
				double X2 = x0 - sqrt(R * R - pow(Y - y0, 2));

				if (l1.PointBelong(Point(X1, Y)) && l2.PointBelong(Point(X1, Y)))
				{
					ans.push_back(Point(X1, Y));
				}

				if (l1.PointBelong(Point(X2, Y)) && l2.PointBelong(Point(X1, Y)))
				{
					ans.push_back(Point(X2, Y));
				}
			}
		}
		else     //Ay+Bx+c = 0
		{
			vector<double> Y = Equation2((A * A) / (B * B) + 1, (2 * A * C) / (B * B) + (2 * A * x0) / B - 2 * y0, (C * C) / (B * B) + (2 * x0 * C) / B - R * R + x0 * x0 + y0 * y0);
			//double CheckA = (A * A) / (B * B) + 1;
			//double CheckB = (2 * A * C) / (B * B) + (2 * A * x0) / B - 2 * y0;
			//double CheckC = (C * C) / (B * B) + (2 * x0 * C) / B - 2 * y0 - R * R + x0 * x0 + y0 * y0;

			for (int i = 0; i < int(Y.size()); i++)
			{
				double X = -(A / B) * Y[i] - (C / B);

				if (l1.PointBelong(Point(X, Y[i])) && l2.PointBelong(Point(X, Y[i])))
				{
					ans.push_back(Point(X, Y[i]));
				}
			}
		}
	}

	return ans;
}
Point Rotate(Point X, Point O, double Ang)   //˳ʱ��
{
	Point ans = Polor(Angle(O, X) + Ang, Distance(O, X), O);
	return ans;
}
Line Rotate(Line L, Point O, double Ang)
{
	Line ans;

	if (L.is_arc)
	{
		ans = Line(Rotate(L.p[0], O, Ang), Rotate(L.p[1], O, Ang), Rotate(L.o, O, Ang), L.color);
	}
	else
	{
		ans = Line(Rotate(L.p[0], O, Ang), Rotate(L.p[1], O, Ang), L.color);
	}

	return ans;
}
double Distance(Point p, Line l)
{
	return abs((l.a * p.y + l.b * p.x + l.c) / (l.a * l.a + l.b * l.b));
}
Line Zoom(Line L, Point c, double Size)
{
	if (L.is_arc)
	{
		Point po = Zoom(L.o, c, Size);
		double rr = L.r * Size;
		return Line(po, L.starta, L.enda, rr, L.color);
	}
	else
	{
		Point p0 = Zoom(L.p[0], c, Size);
		Point p1 = Zoom(L.p[1], c, Size);
		return Line(p0, p1, L.color);
	}
}
class Shape {
	public:
		BYTE fill_type[8];
		vector<Point> p;
		vector<Line> phy_l;
		COLORREF line_color, fill_color;
		bool filling;
		bool close;
		Point center;
		vector<Line> re_l;
		Point last_position;


		Shape()
		{
			phy_l.clear();
			p.clear();
			re_l.clear();
			filling = 0;
			close = 0;
			line_color = WHITE;
			fill_color = WHITE;

			for (int i = 0; i < 8; i++)
			{
				fill_type[i] = 0;
			}
		}
	/*
		Shape& operator=(const Shape& S)
		{
			if (this != &S)
			{
				have_image = S.have_image;
				//image = S.image;
				image_file = S.image_file;
				for(int i=0;i<8;i++)
				{
					fill_type[i] = S.fill_type[i];
				}
				p = S.p;
				phy_l = S.phy_l;
				line_color = S.line_color;
				fill_color = S.fill_color;
				filling = S.filling;
				close = S.close;
				center = S.center;
				re_l = S.re_l;
				last_position = S.last_position;
			} 
			return *this;
		}
	*/
		void SetFullFill()
		{
			filling = 1;

			for (int i = 0; i < 8; i++)
			{
				fill_type[i] = 0b11111111;
			}
		}
		void Debug()
		{
			cout << "-------------------\n";
			cout << "Shape Debug Start:\n";
			cout << "phy_l debug:(" << phy_l.size() << ")\n";

			for (int i = 0; i < int(phy_l.size()); i++)
			{
				phy_l[i].Debug();
			}

			cout << "re_l debug:(" << re_l.size() << "\n";

			for (int i = 0; i < int(re_l.size()); i++)
			{
				re_l[i].Debug();
			}

			cout << "Center Debug:\n";
			center.Debug();
			cout << "filling=" << filling << endl;
			cout << "Shape Debug end\n";
			cout << "-------------------\n";
		}
		void PhyToRe()
		{
			re_l.clear();

			for (int i = 0; i < int(phy_l.size()); i++)
			{
				if (phy_l[i].is_arc)
				{
					re_l.push_back(Line(Point(phy_l[i].o.x - center.x, phy_l[i].o.y - center.y), phy_l[i].starta, phy_l[i].enda, phy_l[i].r, phy_l[i].color));
				}
				else
				{
					re_l.push_back(Line(Point(phy_l[i].p[0].x - center.x, phy_l[i].p[0].y - center.y, phy_l[i].color), Point(phy_l[i].p[1].x - center.x, phy_l[i].p[1].y - center.y, phy_l[i].color), phy_l[i].color));
				}
			}
		}
		void ReToPhy()
		{
			phy_l.clear();

			for (int i = 0; i < int(re_l.size()); i++)
			{
				if (re_l[i].is_arc)
				{
					phy_l.push_back(Line(re_l[i].o.Move(center.x, center.y), re_l[i].starta, re_l[i].enda, re_l[i].r, re_l[i].color));
				}
				else
				{
					phy_l.push_back(Line(re_l[i].p[0].Move(center.x, center.y), re_l[i].p[1].Move(center.x, center.y), re_l[i].color));
				}
			}
		}

		void SetPolygon()
		{
			close = 1;
			phy_l.clear();

			for (int i = 0; i < int(p.size()); i++)
			{
				if (i == int(p.size()) - 1)
				{
					phy_l.push_back(Line(p[i], p[0], line_color));
				}
				else
				{
					phy_l.push_back(Line(p[i], p[i + 1], line_color));
				}
			}

			PhyToRe();
		}

		void SetPolygon(COLORREF color)
		{
			close = 1;
			line_color = color;
			phy_l.clear();
			int p_Size = p.size();
			for (int i = 0; i < p_Size; i++)
			{
				if (i == p_Size - 1)
				{
					phy_l.push_back(Line(p[i], p[0], line_color));
				}
				else
				{
					phy_l.push_back(Line(p[i], p[i + 1], line_color));
				}
			}

			PhyToRe();
		}
		void SetIsogon(double n, Point startp, COLORREF color = WHITE)
		{
			line_color = color;
			p.clear();
			phy_l.clear();
			double PerAng = PI * 2.0 / n;
			double Dis = Distance(center, startp);

			for (double i = 0; i < n; i++)
			{
				Point X = Polor(Angle(center, startp) + i * PerAng, Dis, center);
				X.color = color;
				p.push_back(X);
			}

			SetPolygon(color);
		}
		void SetIsogon(double n, double r, COLORREF color = WHITE)
		{
			line_color = color;
			p.clear();
			phy_l.clear();
			close = 1;
			double PerAng = PI * 2.0 / n;

			for (double i = 0; i < n; i++)
			{
				Point X = Polor(i * PerAng, r, center);
				X.color = color;
				p.push_back(X);
			}

			SetPolygon();
		}
		void SetRectangle(Point P1, COLORREF col = WHITE)
		{
			line_color = col;
			double x0 = center.x, y0 = center.y;
			double x1 = P1.x, y1 = P1.y;
			Point P2(x1, 2 * y0 - y1);
			Point P3(2 * x0 - x1, 2 * y0 - y1);
			Point P4(2 * x0 - x1, y1);
			p.push_back(P1);
			p.push_back(P2);
			p.push_back(P3);
			p.push_back(P4);
			/*



			*/
			SetPolygon(col);
		}
		void SetCircle(double r, COLORREF C = WHITE)
		{
			close = 1;
			phy_l.clear();
			phy_l.push_back(Line(center, 0, 2 * PI, r, C));
			PhyToRe();
		}
		void SetCenter(int XX, int YY)
		{
			center = Point(XX, YY);
			PhyToRe();
		}
		void SetCenter(Point C)
		{
			center = C;
			PhyToRe();
		}
		/*
			void SetImage(string File, Point P1)
			{
				//image_file = File;
				SetRectangle(P1);
				double imagew = 0, imageh = 0;
				for (int i = 0; i < p.size() - 1; i++)
				{
					for (int j = i + 1; j < p.size(); j++)
					{
						imagew = max(abs(p[i].x - p[j].x),imagew);
						imageh = max(abs(p[i].y - p[j].y),imageh);
					}
				}
				//loadimage(&image, _T("image_file"),int(imagew),int(imageh));
				have_image = 1;
			}
		*/
		void Draw()
		{

			ReToPhy();

			for (int i = 0; i < int(phy_l.size()); i++)
			{
				phy_l[i].Draw();
			}

			if (filling)
			{
				setfillstyle(fill_type);
				settextcolor(fill_color);
				floodfill(center.x, center.y, line_color, 0);
			}
		}
		void SetFillColor(COLORREF CC)
		{
			filling = 1;
			fill_color = CC;
		}
		void SetFillType(BYTE* TP)
		{
			filling = 1;

			for (int i = 0; i < 8; i++)
			{
				fill_type[i] = TP[i];
			}
		}
		void SetColor(COLORREF CC)
		{
			line_color = CC;
		}
		void Move(double dx, double dy)
		{
			center.x += dx;
			center.y += dy;
			ReToPhy();
		}
		void MoveTo(double x, double y)
		{
			center = Point(x, y, center.color);
			ReToPhy();
		}
		void PolorMove(double Ang, double dis)
		{
			Move(cos(Ang)*dis, -sin(Ang)*dis);
		}
		bool IsPolygon()
		{
			if (!close)
				return 0;

			for (int i = 0; i < int(phy_l.size()); i++)
			{
				if (phy_l[i].is_arc)
					return 0;
			}

			return 1;
		}

		Shape operator+(Shape S)
		{
			PhyToRe();
			S.PhyToRe();

			for (int i = 0; i < int(re_l.size()); i++)
			{
				S.re_l.push_back(re_l[i]);
			}

			S.ReToPhy();
			return S;
		}
		/*
			void operator=(Shape b)
			{
				center = b.center;
				close = b.close;
				fill_color = b.fill_color;
				fill_type = b.fill_type;
				filling = b.filling;
				have_image = b.have_image;
				image = b.image;
				image_file = b.image_file;
				last_position = b.last_position;
				line_color = b.line_color;
				p = b.p;
				phy_l = b.phy_l;
				re_l = b.re_l;
			}
		*/
};
bool Crash(Shape S, Point p)
{
	Line cl(p, Point(0, 0));
	int ans = 0;
	vector<Point> cp;

	for (int i = 0; i < int(S.phy_l.size()); i++)
	{
		cp = Intersection(cl, S.phy_l[i]);
		ans += cp.size();
	}

	return ans % 2;
}
Shape Rotate(Shape S, Point O, double Ang)
{
	S.ReToPhy();

	for (int i = 0; i < int(S.phy_l.size()); i++)
	{
		S.phy_l[i] = Rotate(S.phy_l[i], O, Ang);
	}

	S.center = Rotate(S.center, O, Ang);
	S.PhyToRe();
	return S;
}
Shape Rotate(Shape S, double Ang)
{
	S.ReToPhy();

	for (int i = 0; i < int(S.phy_l.size()); i++)
	{
		S.phy_l[i] = Rotate(S.phy_l[i], S.center, Ang);
	}

	S.PhyToRe();
	return S;
}
Point Projection(Point P, Line L)   //�㵽ֱ��ͶӰ
{
	if (L.a == 0)
	{
		return Point(-L.c / L.b, P.y);
	}

	if (L.b == 0)
	{
		return Point(P.x, -L.c / L.a);
	}

	return Intersection(L, Line(1, -L.a / L.b, (L.a * P.x / L.b - P.y)))[0];
}
bool Crash(Line L1, Line L2)
{
	vector<Point> cp = Intersection(L1, L2);
	return !cp.empty();
}
int InterNum(Line l1, Line l2)
{
	vector<Point> ans = Intersection(l1, l2);
	return ans.size();
}
bool Crash(Line L, Shape S)
{
	S.ReToPhy();

	for (int i = 0; i < int(S.phy_l.size()); i++)
	{
		if (Crash(L, S.phy_l[i]))
			return 1;
	}

	return 0;
}
bool Crash(Shape A, Shape B)
{
	A.ReToPhy();
	B.ReToPhy();

	for (int i = 0; i < int(A.phy_l.size()); i++)
	{
		for (int j = 0; j < int(B.phy_l.size()); j++)
		{
			if (Crash(A.phy_l[i], B.phy_l[j]))
			{
				return 1;
			}
		}
	}

	bool ans = 0;

	if (A.close)
	{
		ans = ans || Crash(A, B.phy_l[0].p[0]);
	}

	if (B.close)
	{
		ans = ans || Crash(B, A.phy_l[0].p[0]);
	}

	return ans;
}
Shape Zoom(Shape S, Point c, double Size)
{
	S.ReToPhy();

	for (int i = 0; i < int(S.phy_l.size()); i++)
	{
		S.phy_l[i] = Zoom(S.phy_l[i], c, Size);
	}

	S.PhyToRe();
	return S;
}
Shape Zoom(Shape S, double Size)
{
	return Zoom(S, S.center, Size);
}
class Object {
	public:
		Shape crashbox;
		vector<Shape> skin;
		int now_skin;
		Point center;
		double ang;
		Object()
		{
			now_skin = -1;
			//skin.clear();
		}

		void AddSkin(Shape S)
		{
			skin.push_back(S);

			if (now_skin == -1)
				now_skin = 0;
		}

		Shape NowSkin()
		{
			return skin[now_skin];
		}
		void SetSkin(Shape S, int n)
		{
			skin[n] = S;
		}
		void SetCrashbox(Shape ss)
		{
			crashbox = ss;
		}
		void NextSkin()
		{
			if (now_skin < 0)
				return;

			now_skin += 1;
			now_skin %= skin.size();
		}
		void Draw()
		{
			if (now_skin < 0)
				return;

			skin[now_skin].Draw();
		}
		void DrawCrashbox()
		{
			crashbox.Draw();
		}
		void SetCenter(Point C)
		{
			for (int i = 0; i < int(skin.size()); i++)
			{
				skin[i].SetCenter(C);
			}

			crashbox.SetCenter(C);
			center = C;
		}
		void Move(double dx, double dy)
		{
			center.x += dx;
			center.y += dy;

			for (int i = 0; i < int(skin.size()); i++)
			{
				skin[i].Move(dx, dy);
			}

			crashbox.Move(dx, dy);
		}
		void MoveTo(double x, double y)
		{
			center = Point(x, y, center.color);

			for (int i = 0; i < int(skin.size()); i++)
			{
				skin[i].MoveTo(x, y);
			}

			crashbox.MoveTo(x, y);
		}
		void PolorMove(double Ang, double dis)
		{
			Move(cos(Ang)*dis, -sin(Ang)*dis);
		}
		void Go(double dis)
		{
			PolorMove(ang, dis);
		}
		void SelfRotate(double Ang)
		{
			for (int i = 0; i < int(skin.size()); i++)
			{
				skin[i] = Rotate(skin[i], center, Ang);
			}

			crashbox = Rotate(crashbox, center, Ang);
			ang += Ang;
		}
		void SelfZoom(double Size)
		{
			for (int i = 0; i < int(skin.size()); i++)
			{
				skin[i] = Zoom(skin[i], center, Size);
			}

			crashbox = Zoom(crashbox, center, Size);
		}
};
bool Crash(Object a, Object b)
{
	return Crash(a.crashbox, b.crashbox);
}

IMAGE Rotate(IMAGE Img,double Ang)
{
	IMAGE res;
	rotateimage(&res,&Img,Ang,true,true);
	return res;
}

Object Rotate(Object x, Point O, double Ang)
{
	for (int i = 0; i < int(x.skin.size()); i++)
	{
		x.skin[i] = Rotate(x.skin[i], O, Ang);
	}

	x.crashbox = Rotate(x.crashbox, O, Ang);
	return x;
}
Object Rotate(Object x, double Ang)
{
	return Rotate(x, x.center, Ang);
}
vector<bool> CrashEdge(Shape S, Point PUL = Point(0, 0), Point PDR = Point(WinWidth, WinHeight))
{
	Line Edge[4];
	vector<bool> ret;
	ret.push_back(0);
	ret.push_back(0);
	ret.push_back(0);
	ret.push_back(0);
	ret.push_back(0);
	Point PDL = Point(PUL.x, PDR.y);
	Point PUR = Point(PDR.x, PUL.y);
	Edge[0] = Line(PUL, PDL);//Ued
	Edge[1] = Line(PDL, PDR);//Ded
	Edge[2] = Line(PDR, PUR);//Red
	Edge[3] = Line(PUR, PUL);//Led

	for (int i = 0; i < 4; i++)
	{
		if (Crash(Edge[i], S))
		{
			ret[i + 1] = 1;
		}
	}

	int X = S.center.x;
	int Y = S.center.y;

	if (Y >= PDL.y)
	{
		ret[2] = 1;
	}

	if (Y <= PUL.y)
	{
		ret[4] = 1;
	}

	if (X <= PDL.x)
	{
		ret[1] = 1;
	}

	if (X >= PDR.x)
	{
		ret[3] = 1;
	}

	ret[0] = ret[1] || ret[2] || ret[3] || ret[4];
	return ret;
}
vector<bool> CrashEdge(Object B, Point PUL = Point(0, 0), Point PDR = Point(WinWidth, WinHeight))
{
	return CrashEdge(B.crashbox, PUL, PDR);
}
class Button: public Object {
	public:
		string text;
		int last;
		ExMessage msg;
		Shape text_rec;
		Button(string Text)
		{
			last = 0;
			text = Text;
			text_rec.fill_color = BLACK;
		}
		void SetTextRec(Point p)
		{
			text_rec.SetCenter(center);
			text_rec.SetRectangle(p);
		}
		void SetText(string Text)
		{
			text = Text;
		}
		void Draw()
		{
			settextcolor(WHITE);
			RECT r = {int(text_rec.p[0].x), int(text_rec.p[0].y), int(text_rec.p[2].x), int(text_rec.p[2].y)};
			//drawtext(text.c_str(), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			skin[now_skin].Draw();
		}
		bool Press()
		{
			peekmessage(&msg);
			flushmessage();

			if (msg.lbutton)
				return Crash(crashbox, Point(msg.x, msg.y));

			return 0;
		}
		bool Click()
		{
			peekmessage(&msg);
			flushmessage();

			if (msg.lbutton && last == 0)
			{
				last = msg.lbutton;
				return Crash(crashbox, Point(msg.x, msg.y));
			}

			last = msg.lbutton;
			return 0;
		}
};
Object Zoom(Object O, double Size)
{
	O.crashbox = Zoom(O.crashbox, Size);

	for (int i = 0; i < int(O.skin.size()); i++)
	{
		O.skin[i] = Zoom(O.skin[i], Size);
	}

	return O;
}
#endif
