#ifndef CHAMGAME_H
#define CHAMGAME_H
#include "chamtool.h"
#include "chammath.h"
using namespace std;
class Position;
class Scene;
class Window;
class Point;
class Line;
class Shape;
class Entity;
class AABB;

// 位置类
/*
方便传递位置坐标用的工具类
*/
class Position
{
public:
	int x, y;
	Position(int X = 0, int Y = 0)
	{
		x = X;
		y = Y;
	}

	void Set(int X, int Y)
	{
		x = X;
		y = Y;
	}

	Position operator+(const Position &p) const
	{
		return Position(x + p.x, y + p.y);
	}

	Position operator-(const Position &p) const
	{
		return Position(x - p.x, y - p.y);
	}

	Position RelToAbs(Position origin)
	{
		return Position(origin.x + x, origin.y - y);
	}
};

double Distance(Position a, Position b)
{
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

/*场景类
场景是所有实体的最直接的容器，负责管理和渲染这些实体。
实体的坐标均是基于场景原点的相对坐标。
*/
class Scene
{
private:
	int height, width;
	Position origin;

public:
	Scene(int H = 600, int W = 800)
	{
		height = H;
		width = W;
		origin.Set(0, 0);
	}

	int GetHeight()
	{
		return height;
	}

	int GetWidth()
	{
		return width;
	}

	void Print()
	{
	}
};

/*
窗口类
窗口是游戏的顶层容器，负责管理场景和处理用户输入。
窗口处理“真正显示的内容”和“场景内容”的映射关系。
窗口也是用户直接交互的层级
*/

class Window
{
public:
	Scene scene;
	Position scenePos;
	int height, width;
};

/*
包围盒类
用于几何体的快速检测
*/
class AABB
{
public:
	int minX, maxX, minY, maxY;
	AABB(int MinX = 0, int MaxX = 0, int MinY = 0, int MaxY = 0)
	{
		minX = MinX;
		maxX = MaxX;
		minY = MinY;
		maxY = MaxY;
	}

	AABB operator+(const AABB &a) const
	{
		return AABB(min(minX, a.minX), max(maxX, a.maxX), min(minY, a.minY), max(maxY, a.maxY));
	} // 包围盒合并运算

	AABB operator+(const Position &p) const
	{
		return AABB(minX + p.x, maxX + p.x, minY + p.y, maxY + p.y);
	}

	AABB operator-(const Position &p) const
	{
		return AABB(minX - p.x, maxX - p.x, minY - p.y, maxY - p.y);
	}

	void Draw(Position origin, COLORREF color = WHITE)
	{
		rectangle(minX + origin.x, origin.y - minY, maxX + origin.x, origin.y - maxY);
	}
};

// 包围盒碰撞检测
bool Crash(AABB a, AABB b)
{
	if (a.maxX < b.minX || a.minX > b.maxX)
		return false;
	if (a.maxY < b.minY || a.minY > b.maxY)
		return false;
	return true;
}

/*
点类
用于定义线类
*/
class Point
{
public:
	Position pos;

	Point()
	{
		pos.Set(0, 0);
	}

	Point(int x, int y)
	{
		pos.Set(x, y);
	}

	Point(Position p)
	{
		pos = p;
	}
	void Draw(Position origin, COLORREF color = WHITE) // 相对坐标转绝对坐标绘制
	{
		putpixel(origin.x + pos.x, origin.y - pos.y, color);
	}
	AABB GetAABB()
	{
		return AABB(pos.x, pos.x, pos.y, pos.y);
	}
};

double Distance(Point a, Point b)
{
	return Distance(a.pos, b.pos);
}

/*
线类
包含直线与圆弧
*/

class Line
{
public:
	Point a, b;	  // 线的两个端点
	Point center; // 圆弧的圆心
	bool type;	  // 0直线，1圆弧

	Line(Point A, Point B) // 作为直线构建
	{
		a = A;
		b = B;
		type = 0;
	}

	Line(Point C, double StartAngle, double EndAngle, double r) // 作为圆弧角度构建
	{
		center = C;
		a = Point(C.pos.x + r * cos(StartAngle), C.pos.y + r * sin(StartAngle));
		b = Point(C.pos.x + r * cos(EndAngle), C.pos.y + r * sin(EndAngle));
		type = 1;
	}

	void Draw(Position origin, COLORREF color = WHITE)
	{
		setlinecolor(color);
		if (type == 0)
		{
			Position realA = a.pos.RelToAbs(origin);
			Position realB = b.pos.RelToAbs(origin);
			line(realA.x, realA.y, realB.x, realB.y);
		}
		else
		{
			double r = sqrt(pow(a.pos.x - center.pos.x, 2) + pow(a.pos.y - center.pos.y, 2));
			double stangle = atan2(a.pos.y - center.pos.y, a.pos.x - center.pos.x);
			double endangle = atan2(b.pos.y - center.pos.y, b.pos.x - center.pos.x);
			while (endangle < stangle)
				endangle += 2 * PI;
			Position bottom_left = Position(center.pos.x - r, center.pos.y - r).RelToAbs(origin);
			Position top_right = Position(center.pos.x + r, center.pos.y + r).RelToAbs(origin);
			arc(bottom_left.x, top_right.y, top_right.x, bottom_left.y, stangle, endangle);
		}
	}

	double Radius()
	{
		if (type == 1)
		{
			return sqrt(pow(a.pos.x - center.pos.x, 2) + pow(a.pos.y - center.pos.y, 2));
		}
		else
		{
			return 0;
		}
	}

	double *GetParameters()
	{
		if (type == 0)
		{
			double *params = new double[3];

			return params;
		}
		else
		{
			return NULL;
		}
	}

	AABB GetAABB()
	{
		if (type == 0)
		{
			int minX = min(a.pos.x, b.pos.x);
			int maxX = max(a.pos.x, b.pos.x);
			int minY = min(a.pos.y, b.pos.y);
			int maxY = max(a.pos.y, b.pos.y);
			return AABB(minX, maxX, minY, maxY);
		}
		else
		{
			int r = Distance(center, a);
			double stangle = atan2(a.pos.y - center.pos.y, a.pos.x - center.pos.x);
			double endangle = atan2(b.pos.y - center.pos.y, b.pos.x - center.pos.x);
			while (endangle < stangle)
				endangle += 2 * PI;
			AABB ans;
			ans = Line(a, b).GetAABB();

			double angles[4] = {0, PI / 2, PI, 3 * PI / 2};
			for (int i = 0; i < 4; i++)
			{
				if (AngleIsInRange(angles[i], stangle, endangle))
				{
					ans = ans + Point(center.pos.x + r * cos(angles[i]), center.pos.y + r * sin(angles[i])).GetAABB();
				}
			}
			return ans;
		}
	}

	bool Contains(Point p);

	double *Paramater()
	{
		if (type == 0)
		{
			// l: Ax + By + C = 0
			double *params = new double[3];
			double A = b.pos.y - a.pos.y;
			double B = a.pos.x - b.pos.x;
			double C = b.pos.x * a.pos.y - a.pos.x * b.pos.y;
			params[0] = A;
			params[1] = B;
			params[2] = C;
			return params;
		}
		else
		{
			return NULL;
		}
	}
};

double Distance(Line l, Point p) // 点与直线/圆周距离
{
	if (l.type == 0)
	{
		double *para;
		para = l.Paramater();
		double A = para[0];
		double B = para[1];
		double C = para[2];
		return abs(A * p.pos.x + B * p.pos.y + C) / sqrt(A * A + B * B);
	}
	else
	{
		return abs(Distance(l.center.pos, p.pos) - l.Radius());
	}
}

bool Line::Contains(Point p)
{
	if (!EqualZero(Distance(*this, p)))
		return false;
	if (type == 0)
	{
		return IsInRange(p.pos.x, min(a.pos.x, b.pos.x), max(a.pos.x, b.pos.x)) &&
			   IsInRange(p.pos.y, min(a.pos.y, b.pos.y), max(a.pos.y, b.pos.y));
	}
	else
	{
		double angleP = atan2(p.pos.y - center.pos.y, p.pos.x - center.pos.x);
		double stangle = atan2(a.pos.y - center.pos.y, a.pos.x - center.pos.x);
		double endangle = atan2(b.pos.y - center.pos.y, b.pos.x - center.pos.x);
		while (endangle < stangle)
			endangle += 2 * PI;
		return AngleIsInRange(angleP, stangle, endangle);
	}
}

bool Crash(Line a, Line b)
{
	AABB A = a.GetAABB();
	AABB B = b.GetAABB();
	if (!Crash(A, B))
		return false;
	else
	{
		if (a.type == 0 && b.type == 0)
		{
			double *paraA, *paraB;
			paraA = a.Paramater();
			paraB = b.Paramater();
			if (paraA[0] * paraB[1] - paraB[0] * paraA[1] == 0) // 平行
				return 0;
			else
			{
				Matrix A(2, 2);
				Matrix C(2, 1);
				A.data[0][0] = paraA[0];
				A.data[0][1] = paraA[1];
				A.data[1][0] = paraB[0];
				A.data[1][1] = paraB[1];
				C.data[0][0] = -paraA[2];
				C.data[1][0] = -paraB[2];
				Matrix ans = A.Inverse() * C;
				Point intersectionPoint(ans.data[0][0], ans.data[1][0]);
				return a.Contains(intersectionPoint) && b.Contains(intersectionPoint);
			}
		}
		else if (a.type == 1 && b.type == 1)
		{
			if (Distance(a.center.pos, b.center.pos) > a.Radius() + b.Radius())
			{
				return false;
			}
			return false;
		}
		else
		{
			// 直线与圆弧碰撞检测
			// 待实现
			return false;
		}
	}
}

class Shape
{
public:
	vector<Line> lines;

	AABB GetAABB()
	{
		AABB ans;
		if (lines.size() == 0)
			return ans;
		ans = lines[0].GetAABB();
		for (size_t i = 1; i < lines.size(); i++)
		{
			ans = ans + lines[i].GetAABB();
		}
		return ans;
	}
};

bool Crash(Shape a, Shape b)
{
	AABB A = a.GetAABB();
	AABB B = b.GetAABB();
	return Crash(A, B);
}

class Entity
{
public:
	Position pos;
	Shape CrashBox;
	vector<IMAGE> skins;
	IMAGE skin;
};

#endif
