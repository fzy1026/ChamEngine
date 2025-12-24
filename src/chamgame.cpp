#include "../include/chamgame.h"
#include "../include/chammath.h"
using namespace std;

// 位置类
/*
方便传递位置坐标用的工具类
*/

Position::Position(int X, int Y)
{
	x = X;
	y = Y;
}

void Position::Set(int X, int Y)
{
	x = X;
	y = Y;
}

Position Position::operator+(const Position &p) const
{
	return Position(x + p.x, y + p.y);
}

Position Position::operator-(const Position &p) const
{
	return Position(x - p.x, y - p.y);
}

Position Position::RelToAbs(Position origin)
{
	return Position(origin.x + x, origin.y - y);
}

double Distance(Position a, Position b)
{
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

AABB::AABB(int MinX, int MaxX, int MinY, int MaxY)
{
	minX = MinX;
	maxX = MaxX;
	minY = MinY;
	maxY = MaxY;
}

AABB AABB::operator+(const AABB &a) const
{
	return AABB(min(minX, a.minX), max(maxX, a.maxX), min(minY, a.minY), max(maxY, a.maxY));
} // 包围盒合并运算

AABB AABB::operator+(const Position &p) const
{
	return AABB(minX + p.x, maxX + p.x, minY + p.y, maxY + p.y);
}


void AABB::Draw(Position origin, COLORREF color)
{
	setlinecolor(color);
	rectangle(minX + origin.x, origin.y - minY, maxX + origin.x, origin.y - maxY);
}

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
Point::Point()
{
	pos.Set(0, 0);
}

Point::Point(int x, int y)
{
	pos.Set(x, y);
}

Point::Point(Position p)
{
	pos = p;
}

void Point::Draw(Position origin, COLORREF color) // 相对坐标转绝对坐标绘制
{
	setlinecolor(color);
	setfillcolor(color);
	fillcircle(origin.x + pos.x, origin.y - pos.y,2);
}

AABB Point::GetAABB()
{
	return AABB(pos.x, pos.x, pos.y, pos.y);
}

void Point::Move(Position delta)
{
	pos = pos + delta;
}

void Point::Move(int dx, int dy)
{
	Position delta(dx, dy);
	Move(delta);
}

void Point::MoveTo(Position newPos)
{
	pos = newPos;
}

void Point::MoveTo(int x, int y)
{
	pos.Set(x, y);
}

void Point::Rotate(Point center, double angle) // 以center为中心逆时针旋转angle弧度
{
	double a = atan2(pos.y - center.pos.y, pos.x - center.pos.x);
	a += angle;
	double r = Distance(pos, center.pos);
	MoveTo(center.pos.x + r * cos(a), center.pos.y + r * sin(a));
}

void Point::Zoom(Point center, double scale) // 以center为中心缩放scale倍
{
	double a = atan2(pos.y - center.pos.y, pos.x - center.pos.x);
	double r = Distance(pos, center.pos);
	MoveTo(center.pos.x + r * scale * cos(a), center.pos.y + r * scale * sin(a));
}

double Distance(Point a, Point b)
{
	return Distance(a.pos, b.pos);
}

/*
线类
包含直线与圆弧
*/

Line::Line(Point A, Point B) // 作为直线构建
{
	a = A;
	b = B;
	type = 0;
}

Line::Line(Point C, double StartAngle, double EndAngle, double r) // 作为圆弧角度构建
{
	center = C;
	a = Point(C.pos.x + r * cos(StartAngle), C.pos.y + r * sin(StartAngle));
	b = Point(C.pos.x + r * cos(EndAngle), C.pos.y + r * sin(EndAngle));
	type = 1;
}

void Line::Move(Position delta)
{
	a.Move(delta);
	b.Move(delta);
	if (type == 1)
		center.Move(delta);
}

void Line::Move(int dx, int dy)
{
	Position delta(dx, dy);
	Move(delta);
}

void Line::Rotate(Point centerPoint, double angle) // 以centerPoint为中心逆时针旋转angle弧度
{
	a.Rotate(centerPoint, angle);
	b.Rotate(centerPoint, angle);
	if (type == 1)
		center.Rotate(centerPoint, angle);
}

void Line::Zoom(Point centerPoint, double scale) // 以centerPoint为中心缩放scale倍
{
	a.Zoom(centerPoint, scale);
	b.Zoom(centerPoint, scale);
	if (type == 1)
		center.Zoom(centerPoint, scale);
}

void Line::Draw(Position origin, COLORREF color)
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

double Line::Radius()
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

double *Line::GetParameters()
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

AABB Line::GetAABB()
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

double *Line::Paramater()
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

Shape::Shape(vector<Line> L)
{
	lines = L;
}

Shape::Shape()
{
	lines.clear();
}

AABB Shape::GetAABB()
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

void Shape::Draw(Position origin, COLORREF color)
{
	for (size_t i = 0; i < lines.size(); i++)
	{
		lines[i].Draw(origin, color);
	}
}

void Shape::Rotate(Point centerPoint, double angle) // 以centerPoint为中心逆时针旋转angle弧度
{
	for (size_t i = 0; i < lines.size(); i++)
	{
		lines[i].Rotate(centerPoint, angle);
	}
}

void Shape::Zoom(Point centerPoint, double scale) // 以centerPoint为中心缩放scale倍
{
	for (size_t i = 0; i < lines.size(); i++)
	{
		lines[i].Zoom(centerPoint, scale);
	}
}

bool Crash(Shape a, Shape b)
{
	AABB A = a.GetAABB();
	AABB B = b.GetAABB();
	return Crash(A, B);
}

void Entity::SetPosition(int x, int y)
{
	pos.Set(x, y);
}

void Entity::SetPosition(Position p)
{
	pos = p;
}

void Entity::SetCrashBox(Shape box)
{
	CrashBox = box;
}

Position Entity::GetPosition()
{
	return pos;
}

Shape Entity::GetCrashBox()
{
	return CrashBox;
}

void Entity::SetSkin(int index)
{
	if (index >= 0 && index < skins.size())
	{
		skin = skins[index];
	}
}

void Entity::DelSkin(int index)
{
	if (index >= 0 && index < skins.size())
	{
		skins.erase(skins.begin() + index);
	}
}

int Entity::AddSkin(IMAGE img)
{
	skins.push_back(img);
	return skins.size()-1;
}

int Entity::AddSkin(string path)
{
	IMAGE img;
	loadimage(&img, (LPCSTR)path.c_str());
	skins.push_back(img);
	return skins.size() - 1;
}

void Entity::Draw(Position origin)
{
	Position realPos = pos.RelToAbs(origin);
	putimage(realPos.x, realPos.y, &skin);
}



bool Crash(Entity a, Entity b)
{
	return Crash(a.CrashBox, b.CrashBox);
}

Scene::Scene(int H, int W)
{
	height = H;
	width = W;
	origin.Set(0, 0);
}

int Scene::GetHeight()
{
	return height;
}

int Scene::GetWidth()
{
	return width;
}

void Scene::Draw()
{
	
}

