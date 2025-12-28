#ifndef CHAMGAME_H
#define CHAMGAME_H
#include "chamtool.h"
#include "chammath.h"
using namespace std;
class Object;
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
	Position operator*(const int &p) const;
	Position operator*(const long &p) const;
	Position operator*(const long long &p) const;
	Position operator/(const int &p) const;
	Position operator/(const long &p) const;
	Position operator/(const long long &p) const;

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

class Object
{
public:
	Position pos;
	void Draw(Position origin, COLORREF color = WHITE); // 相对坐标转绝对坐标绘制
	void Move(Position delta);
	void Move(int dx, int dy);
	void MoveTo(Position newPos);
	void MoveTo(int x, int y);
	void Rotate(Point center, double angle); // 以center为中心逆时针旋转angle弧度
	void Zoom(Point center, double scale);	 // 以center为中心缩放scale倍
};

/*
点类
用于定义线类
*/
class Point : public Object
{
public:
	Point();
	Point(int x, int y);
	Point(Position p);
	AABB GetAABB();
	void Draw(Position origin, COLORREF color = WHITE); // 相对坐标转绝对坐标绘制
	void Move(Position delta);
	void Move(int dx, int dy);
	void MoveTo(Position newPos);
	void MoveTo(int x, int y);
	void Rotate(Point center, double angle); // 以center为中心逆时针旋转angle弧度
	void Zoom(Point center, double scale);	 // 以center为中心缩放scale倍
};

double Distance(Point a, Point b);

Point MidPoint(Point a, Point b);

/*
线类
包含直线与圆弧
*/

class Line : public Object
{
public:
	Point a, b;	  // 线的两个端点
	Point center; // 圆弧的圆心
	bool type;	  // 0直线，1圆弧

	Line(Point A, Point B);										 // 作为直线构建
	Line(Point C, double StartAngle, double EndAngle, double r); // 作为圆弧角度构建
	double Radius();
	double *GetParameters();
	AABB GetAABB();
	bool Contains(Point p);
	double *Paramater();
	void Draw(Position origin, COLORREF color = WHITE); // 相对坐标转绝对坐标绘制
	void Move(Position delta);
	void Move(int dx, int dy);
	void MoveTo(Position newPos);
	void MoveTo(int x, int y);
	void Rotate(Point center, double angle); // 以center为中心逆时针旋转angle弧度
	void Zoom(Point center, double scale);	 // 以center为中心缩放scale倍
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
	void Draw(Position origin, COLORREF color = WHITE); // 相对坐标转绝对坐标绘制
	void Move(Position delta);
	void Move(int dx, int dy);
	void MoveTo(Position newPos);
	void MoveTo(int x, int y);
	void Rotate(Point center, double angle); // 以center为中心逆时针旋转angle弧度
	void Zoom(Point center, double scale);	 // 以center为中心缩放scale倍
	
};

bool Crash(Shape a, Shape b);

//
class Image : public Object
{
public:
	string address;
	IMAGE image;
	int height, width;
	Image(string Address, int Height = 0, int Width = 0);
	Image(IMAGE NewImage, int Height = 0, int Width = 0);
	void SetSize(int Height, int Width);
	void Draw(Position origin, COLORREF color = WHITE); // 相对坐标转绝对坐标绘制
	void Move(Position delta);
	void Move(int dx, int dy);
	void MoveTo(Position newPos);
	void MoveTo(int x, int y);
	void Rotate(Point center, double angle); // 以center为中心逆时针旋转angle弧度
	void Zoom(Point center, double scale);	 // 以center为中心缩放scale倍
};

class Entity : public Object
{
public:
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
	void Draw(Position origin,COLORREF color = WHITE);//相对坐标转绝对坐标绘制
	void Move(Position delta);
	void Move(int dx, int dy);
	void MoveTo(Position newPos);
	void MoveTo(int x, int y);
	void Rotate(Point center, double angle); // 以center为中心逆时针旋转angle弧度
	void Zoom(Point center, double scale); // 以center为中心缩放scale倍
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
	Position origin; // 原点在EasyX的绝对坐标

public:
	Scene(int H = 600, int W = 800);
	int GetHeight();
	int GetWidth();
	void Draw();
};

#endif
