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
	Position(int X = 0, int Y = 0);
	void Set(int X, int Y);
	Position operator+(const Position &p) const;
	Position operator-(const Position &p) const;
	Position RelToAbs(Position origin);
};

double Distance(Position a, Position b);

/*
包围盒类
用于几何体的快速检测
*/
class AABB
{
public:
	int minX, maxX, minY, maxY;
	AABB(int MinX = 0, int MaxX = 0, int MinY = 0, int MaxY = 0);
	AABB operator+(const AABB &a) const; // 包围盒合并运算
	AABB operator+(const Position &p) const;
	void Draw(Position origin, COLORREF color = WHITE);
};

// 包围盒碰撞检测
bool Crash(AABB a, AABB b);

/*
点类
用于定义线类
*/
class Point
{
public:
	Position pos;

	Point();

	Point(int x, int y);

	Point(Position p);
	void Draw(Position origin, COLORREF color = WHITE); // 相对坐标转绝对坐标绘制
	AABB GetAABB();
	void Move(Position delta);
	void Move(int dx, int dy);
	void MoveTo(Position newPos);
	void MoveTo(int x, int y);
	void Rotate(Point center, double angle); // 以center为中心逆时针旋转angle弧度
	void Zoom(Point center, double scale); // 以center为中心缩放scale倍
	
};

double Distance(Point a, Point b);

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

	Line(Point A, Point B); // 作为直线构建
	Line(Point C, double StartAngle, double EndAngle, double r); // 作为圆弧角度构建
	void Move(Position delta);
	void Move(int dx, int dy);
	void Rotate(Point centerPoint, double angle); // 以centerPoint为中心逆时针旋转angle弧度
	void Zoom(Point centerPoint, double scale); // 以centerPoint为中心缩放scale倍
	void Draw(Position origin, COLORREF color = WHITE);
	double Radius();
	double *GetParameters();
	AABB GetAABB();
	bool Contains(Point p);
	double *Paramater();
};

double Distance(Line l, Point p); // 点与直线/圆周距离


bool Crash(Line a, Line b);

class Shape
{
public:
	vector<Line> lines;

	Shape(vector<Line> L);
	Shape();
	AABB GetAABB();
	void Draw(Position origin, COLORREF color = WHITE);
	void Rotate(Point centerPoint, double angle); // 以centerPoint为中心逆时针旋转angle弧度
	void Zoom(Point centerPoint, double scale); // 以centerPoint为中心缩放scale倍
};

bool Crash(Shape a, Shape b);

class Entity
{
public:
	Position pos;
	Shape CrashBox;
	vector<IMAGE> skins;
	IMAGE skin;

	void SetPosition(int x, int y);
	void SetPosition(Position p);
	void SetCrashBox(Shape box);
	Position GetPosition();
	Shape GetCrashBox();
	void SetSkin(int index);
	void DelSkin(int index);
	int AddSkin(IMAGE img);
	int AddSkin(string path);
	void Draw(Position origin);
};

bool Crash(Entity a, Entity b);

/*场景类
场景是所有实体的最直接的容器，负责管理和渲染这些实体。
实体的坐标均是基于场景原点的相对坐标。
*/
class Scene
{
private:
	int height, width;
	Position origin;//原点在EasyX的绝对坐标

public:
	Scene(int H = 600, int W = 800);
	int GetHeight();
	int GetWidth();
	void Draw();
};

#endif
