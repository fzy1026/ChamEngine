#ifndef CHAMGAME_H
#define CHAMGAME_H
#include "chammath.h"
#include <easyx/easyx.h>
#include <easyx/graphics.h>
using namespace std;
class Point;
class Scene;
class Window;
class Point;
class Line;
class Shape;
class Entity;
class AABB;

double Distance(Point a, Point b);

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
	AABB operator+(const Point &p) const;
	void Draw(Point origin, COLORREF color = WHITE);
};

// 包围盒碰撞检测
bool Crash(AABB a, AABB b);

/*
点类
用于定义线类,以及传递坐标
*/
class Point
{
public:
	int x,y;
	Point();
	Point(int X, int Y);
	AABB GetAABB();
	void Set(int X,int Y);
	void Draw(Point origin, COLORREF color = WHITE); // 相对坐标转绝对坐标绘制
	void Move(Point delta);
	void Move(int dx, int dy);
	void Rotate(Point center, double angle); // 以center为中心逆时针旋转angle弧度
	void Zoom(Point center, double scale);	// 以center为中心缩放scale倍
	Point AbsToRel(Point origin);//相对坐标转绝对坐标
	Point RelToAbs(Point origin);//绝对坐标转相对坐标
};

double Distance(Point a, Point b);

Point MidPoint(Point a, Point b);

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

	Line(Point A, Point B);										 // 作为直线构建
	Line(Point C, double StartAngle, double EndAngle, double r); // 作为圆弧角度构建
	double Radius();
	double *GetParameters();
	AABB GetAABB();
	bool Contains(Point p);
	double *Paramater();//p[0]x+p[1]y+p[2]=0
	void Draw(Point origin, COLORREF color = WHITE); // 相对坐标转绝对坐标绘制
	void Move(Point delta);
	void Move(int dx, int dy);
	void MoveTo(Point newPos);
	void MoveTo(int x, int y);
	void Rotate(Point center, double angle); // 以center为中心逆时针旋转angle弧度
	void Zoom(Point center, double scale);	// 以center为中心缩放scale倍
	bool AngleContains(double angle);//某个角度是否被包含在弧线中
	Point Projection(Point p);//p点在直线上的投影
	double Angle();//线段方位角(与X轴夹角)
	double StartAngle();//圆弧起始角
	double EndAngle();//圆弧中止角
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
	void Draw(Point origin, COLORREF color = WHITE); // 相对坐标转绝对坐标绘制
	void Move(Point delta);
	void Move(int dx, int dy);
	void MoveTo(Point newPos);
	void MoveTo(int x, int y);
	void Rotate(Point center, double angle); // 以center为中心逆时针旋转angle弧度
	void Zoom(Point center, double scale);	// 以center为中心缩放scale倍
};

bool Crash(Shape a, Shape b);

//
class Image
{
public:
	string address;
	IMAGE image;
	int height, width;
	Point pos; // 绘制位置(相对坐标)
	Image(string Address, int Height = 0, int Width = 0);
	void SetSize(int Height, int Width);
	void Draw(Point origin, DWORD dwrop = SRCCOPY); // 相对坐标转绝对坐标绘制
	void Move(int dx, int dy);
	void MoveTo(Point newPos);
	void MoveTo(int x, int y);
	void Rotate(Point center, double angle); // 逆时针旋转angle弧度
	void Zoom(Point center, double scale);	// 以center为中心缩放scale倍
};

class Entity
{
public:
	Shape CrashBox;
	vector<Image> skins;
	int skinIndex;
	Point pos; // 实体原点(所有实体所包含的元素均以此点绝对坐标为原点)

	void SetPosition(int x, int y);
	void SetPosition(Point p);
	void SetCrashBox(Shape box);
	Point GetPosition();
	Shape GetCrashBox();
	void SetSkin(int index);
	void DelSkin(int index);
	int AddSkin(Image img);
	int AddSkin(string path);
	void Draw(Point origin, COLORREF color = WHITE); // 相对坐标转绝对坐标绘制
	void Move(Point delta);
	void Move(int dx, int dy);
	void MoveTo(Point newPos);
	void MoveTo(int x, int y);
	void Rotate(Point center, double angle); // 以center为中心逆时针旋转angle弧度
	void Zoom(Point center, double scale);	 // 以center为中心缩放scale倍
	Image* CurrentSkin();
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
	double scale;//场景放缩比例
	Point origin; // 原点在EasyX的绝对坐标

public:
	Scene(int H = 600, int W = 800);
	int GetHeight();
	int GetWidth();
	void Draw();
};

#endif
