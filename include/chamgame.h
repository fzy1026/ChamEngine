#ifndef CHAMGAME_H
#define CHAMGAME_H
#include "chammath.h"
#include <easyx/easyx.h>
#include <easyx/graphics.h>
using namespace std;
/*
一些烦人的坐标系细节：
相对坐标：以某个自定义点为原点的坐标系，x轴向右为正，y轴向上为正
绝对坐标：以窗口左下角为原点的坐标系，坐标轴方向同相对坐标
EasyX坐标：以窗口右上角为原点的坐标系，x轴向右为正，y轴向下为正，这是一切EasyX库的，直接绘图函数采用的坐标系
*/
class AABB;

extern int GRAPH_WIDTH, GRAPH_HEIGHT; // 窗口长宽，坐标系转换用

void InitEngine(int width, int height, int flag = 0);



/*
点类
用于定义线类,以及传递坐标
*/
class Point
{
public:
	int x, y;
	Point();
	Point(int X, int Y);
	AABB GetAABB();
	void Set(int X, int Y);
	void Draw(Point origin = Point(0, 0), COLORREF color = WHITE); // 相对坐标转绝对坐标绘制
	void Move(Point delta);
	void Move(int dx, int dy);
	void Rotate(Point center, double angle); // 以center为中心逆时针旋转angle弧度
	void Zoom(Point center, double scale);	 // 以center为中心缩放scale倍
	Point AbsToRel(Point origin);			 // 相对坐标转绝对坐标
	Point RelToAbs(Point origin);			 // 绝对坐标转相对坐标
	Point ToEasyX();						 // 转化为EasyX坐标系的实际绘制坐标,很烦人。
};

double Distance(Point a, Point b); // 不建议直接使用这个函数，除非你确定两个点在同一坐标系下


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
	void Draw(Point origin = Point(0, 0), COLORREF color = WHITE);
};

// 包围盒碰撞检测
bool Crash(AABB a, AABB b);



Point MidPoint(Point a, Point b);

/*
线类
包含直线与圆弧
*/

class Line
{
public:
	Point a, b;	  // 线的两个端点(相对坐标)
	Point center; // 圆弧的圆心(相对坐标)
	bool type;	  // 0直线，1圆弧

	Line(Point A, Point B);										 // 作为直线构建
	Line(Point C, double StartAngle, double EndAngle, double r); // 作为圆弧角度构建
	double Radius();
	double *GetParameters();
	AABB GetAABB();
	bool Contains(Point p);
	double *Paramater();										   // p[0]x+p[1]y+p[2]=0
	void Draw(Point origin = Point(0, 0), COLORREF color = WHITE); // 以origin为相对坐标系零点，转绝对坐标绘制
	void Move(Point delta);
	void Move(int dx, int dy);
	void MoveTo(Point newPos);
	void MoveTo(int x, int y);
	void Rotate(Point center, double angle); // 以center(相对坐标)为中心逆时针旋转angle弧度
	void Zoom(Point center, double scale);	 // 以center(相对坐标)为中心缩放scale倍
	bool AngleContains(double angle);		 // 某个角度是否被包含在弧线中
	Point Projection(Point p);				 // p点在直线上的投影
	double Angle();							 // 线段方位角(与X轴夹角)
	double StartAngle();					 // 圆弧起始角
	double EndAngle();						 // 圆弧中止角
	Line AbsToRel(Point origin);			 // 相对坐标转绝对坐标
	Line RelToAbs(Point origin);			 // 绝对坐标转相对坐标
};

double Distance(Line l, Point p); // 点与直线/圆周距离

bool Crash(Line a, Line b); // 不建议使用这个函数,除非你确定他们处于同一坐标系下！

class Shape
{
public:
	vector<Line> lines;

	Shape(vector<Line> L);
	Shape();
	AABB GetAABB();
	void Draw(Point origin = Point(0, 0), COLORREF color = WHITE); // 以origin为相对坐标系原点,转绝对坐标绘制
	void Move(Point delta);
	void Move(int dx, int dy);
	void MoveTo(Point newPos);
	void MoveTo(int x, int y);
	void Rotate(Point center, double angle); // 以center(相对坐标)为中心逆时针旋转angle弧度
	void Zoom(Point center, double scale);	 // 以center(相对坐标)为中心缩放scale倍
	Shape AbsToRel(Point origin);			 // 相对坐标转绝对坐标
	Shape RelToAbs(Point origin);			 // 绝对坐标转相对坐标
};

bool Crash(Shape a, Shape b); // 不建议使用这个函数,除非你确定他们处于同一坐标系下！

//
class Image
{
public:
	string address;
	IMAGE image;
	int height, width;
	double ang; // 图片旋转的角度,保证重载后旋转状态不变
	Image(string Address, int Height = 0, int Width = 0);
	void SetSize(int Height, int Width);						  // 更改图片尺寸
	void Load();												  // 按照当前尺寸与角度,从Address处加载图片
	void SetAddress(string Address);							  // 更改加载地址
	void Draw(Point origin = Point(0, 0), DWORD dwrop = SRCCOPY); // 以origin(绝对坐标)为图片中心，绘制上一次Load的图片
	void Rotate(double angle);									  // 逆时针旋转angle弧度
	void SetAng(double angle);									  // 设置图片旋转角度
	void Zoom(double scale);									  // 缩放scale倍
};

class Entity
{
public:
	Shape CrashBox;
	vector<Image> skins;
	int skinIndex;
	Point pos; // 实体中心，实体的碰撞箱与皮肤均以pos为相对坐标系原点

	void SetPosition(int x, int y);
	void SetPosition(Point p);
	void SetCrashBox(Shape box);
	Point GetPosition();
	Shape GetCrashBox();
	void SetSkin(int index);
	void DelSkin(int index);
	int AddSkin(Image img);
	int AddSkin(string path);
	void Draw(Point origin = Point(0, 0)); // 以origin为pos绝对坐标，转EasyX坐标绘制
	void Move(int dx, int dy);
	void MoveTo(Point newPos);
	void MoveTo(int x, int y);
	void Rotate(Point center, double angle); // 以center(相对坐标)为中心逆时针旋转angle弧度
	void Zoom(Point center, double scale);	 // 以center(相对坐标)为中心缩放scale倍
	Image *CurrentSkin();
};

bool Crash(Entity a, Entity b);//不建议直接使用这个函数，除非你确定两个实体处于同一坐标系下！

/*场景类
场景是所有实体的最直接的容器，负责管理和渲染这些实体。
实体的坐标均是基于场景原点的相对坐标。
*/
class Scene
{
public:
	double scale;						  // 场景放缩比例
	Point origin;						  // 原点在EasyX的绝对坐标
	vector<pair<Entity *, int>> entities; // 场景所管理的实体，以及其对应的优先级
	int maxPriority;					  // 记录目前的最高优先级
	Scene(Point p = Point(0, 0));
	void SetOrigin(Point p);
	void Draw();									 // 优先级高（相同时索引靠前）的实体后绘制
	int AddEntity(Entity *entity, int priority = 0); // 返回加入的实体（加在尾部）的索引
	void SetEntity(Entity *entity, int index);		 // 更改某索引对应的实体
	void DelEntity(int index);						 // 删除某索引对应的实体，这一操作可能导致其他实体索引更改！
	int GetIndex(Entity *entity);					 // 查找实体对应索引,-1代表未找到
	void SetPriority(int p, int index);				 // 设置某实体优先级
	void SetScale(double Scale);
	// TODO:查找部分也许可以加优化....?
};

#endif
