#include "../include/chamgame.h"
#include "../include/chammath.h"
using namespace std;

short KeyState[256],LastKeyState[256];// 键盘状态数组(当下&上一帧)
const bool KEY_DOWN = 1, KEY_UP = 0; // 按键状态常量
int GRAPH_WIDTH, GRAPH_HEIGHT;

void InitEngine(int width, int height, int flag)
{
	GRAPH_WIDTH = width;
	GRAPH_HEIGHT = height;
	initgraph(width, height, flag);
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
	return AABB(std::min(minX, a.minX), std::max(maxX, a.maxX), std::min(minY, a.minY), std::max(maxY, a.maxY));
} // 包围盒合并运算

void AABB::Draw(Point origin, COLORREF color)
{
	setlinecolor(color);
	rectangle(minX, minY, maxX, maxY);
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
	x = 0;
	y = 0;
}

Point::Point(int X, int Y)
{
	x = X;
	y = Y;
}

void Point::Draw(Point origin, COLORREF color) // 相对坐标转EasyX坐标绘制
{
	setlinecolor(color);
	setfillcolor(color);
	Point EasyXP = Point(x, y).AbsToRel(origin).ToEasyX();
	fillcircle(EasyXP.x, EasyXP.y, 2);
}

AABB Point::GetAABB()
{
	return AABB(x, x, y, y);
}

void Point::Move(Point delta)
{
	x += delta.x;
	y += delta.y;
}

void Point::Move(int dx, int dy)
{
	x += dx;
	y += dy;
}

void Point::Set(int X, int Y)
{
	x = X;
	y = Y;
}

void Point::Rotate(Point center, double angle) // 以center为中心逆时针旋转angle弧度
{
	double a = atan2(y - center.y, x - center.x);
	a += angle;
	double r = Distance(*this, center);
	Set(center.x + r * cos(a), center.y + r * sin(a));
}

Point Point::Zoom(Point center, double scale) // 以center为中心缩放scale倍
{
	double a = atan2(y - center.y, x - center.x);
	double r = Distance(*this, center);
	r *= scale;
	return Point(center.x + r * cos(a), center.y + r * sin(a));
}

Point Point::RelToAbs(Point origin)
{
	return Point(origin.x + x, origin.y + y);
}

Point Point::AbsToRel(Point origin)
{
	return Point(x - origin.x, y - origin.y);
}

Point Point::ToEasyX()
{
	return Point(x, GRAPH_HEIGHT - y);
}

double Distance(Point a, Point b)
{
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

Point MidPoint(Point a, Point b)
{
	return Point((a.x + b.x) / 2, (a.y + b.y) / 2);
}

Point MousePos()
{
	Point p;
	ExMessage m = getmessage(EM_MOUSE);
	p.x = m.x;
	p.y = GRAPH_HEIGHT - m.y;
	return p;
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

Line::Line(Point C,Point A,Point B)
{
	if(Distance(C,A)!= Distance(C,B))
		throw "|CA| != |CB| when building a arc.";
	center = C;
	a = A;
	b = B;
	type = 1;

}

Line::Line(Point C, double StartAngle, double EndAngle, double r) // 作为圆弧角度构建
{
	if (EndAngle < StartAngle)
		swap(StartAngle, EndAngle);
	center = C;
	a = Point(C.x + r * cos(StartAngle), C.y + r * sin(StartAngle));
	b = Point(C.x + r * cos(EndAngle), C.y + r * sin(EndAngle));
	type = 1;
}

void Line::Move(Point delta)
{
	a.Move(delta);
	b.Move(delta);
	if (type == 1)
		center.Move(delta);
}

void Line::Move(int dx, int dy)
{
	Point delta(dx, dy);
	Move(delta);
}

void Line::Rotate(Point centerPoint, double angle)
{
	a.Rotate(centerPoint, angle);
	b.Rotate(centerPoint, angle);
	if (type == 1)
		center.Rotate(centerPoint, angle);
}

Line Line::Zoom(Point centerPoint, double scale)
{
	Point Zooma = a.Zoom(centerPoint, scale);
	Point Zoomb = b.Zoom(centerPoint, scale);
	if (type == 1)
	{
		Point Zoomcenter = center.Zoom(centerPoint, scale);
		return Line(Zoomcenter,Zooma,Zoomb);
	}
	else
	{
		return Line(Zooma,Zoomb);
	}
}

void Line::Draw(Point origin, COLORREF color)
{
	setlinecolor(color);
	if (type == 0)
	{
		Point realA = a.RelToAbs(origin).ToEasyX();
		Point realB = b.RelToAbs(origin).ToEasyX();
		line(realA.x, realA.y, realB.x, realB.y);
	}
	else
	{
		double r = Distance(center, a);
		double stangle = atan2(a.y - center.y, a.x - center.x);
		double endangle = atan2(b.y - center.y, b.x - center.x);
		while (endangle < stangle)
			endangle += 2 * PI;
		Point bottom_left = Point(center.x - r, center.y - r).RelToAbs(origin).ToEasyX();
		Point top_right = Point(center.x + r, center.y + r).RelToAbs(origin).ToEasyX();
		arc(bottom_left.x, top_right.y, top_right.x, bottom_left.y, stangle, endangle);
	}
}

double Line::Radius()
{
	if (type == 1)
	{
		return sqrt(pow(a.x - center.x, 2) + pow(a.y - center.y, 2));
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
		params[0] = b.y - a.y;
		params[1] = b.x - a.x;
		params[2] = b.y * a.x - a.y * b.x;
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
		int minX = std::min(a.x, b.x);
		int maxX = std::max(a.x, b.x);
		int minY = std::min(a.y, b.y);
		int maxY = std::max(a.y, b.y);
		return AABB(minX, maxX, minY, maxY);
	}
	else
	{
		int r = Distance(center, a);
		double stangle = atan2(a.y - center.y, a.x - center.x);
		double endangle = atan2(b.y - center.y, b.x - center.x);
		while (endangle < stangle)
			endangle += 2 * PI;
		AABB ans;
		ans = Line(a, b).GetAABB();

		double angles[4] = {0, PI / 2, PI, 3 * PI / 2};
		for (int i = 0; i < 4; i++)
		{
			if (AngleIsInRange(angles[i], stangle, endangle))
			{
				ans = ans + Point(center.x + r * cos(angles[i]), center.y + r * sin(angles[i])).GetAABB();
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
		double A = b.y - a.y;
		double B = a.x - b.x;
		double C = b.x * a.y - a.x * b.y;
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
		return abs(A * p.x + B * p.y + C) / sqrt(A * A + B * B);
	}
	else
	{
		return abs(Distance(l.center, p) - l.Radius());
	}
}

bool Line::Contains(Point p)
{
	if (!EqualZero(Distance(*this, p)))
		return false;
	if (type == 0)
	{
		return InRange(p.x, std::min(a.x, b.x), std::max(a.x, b.x)) &&
			   InRange(p.y, std::min(a.y, b.y), std::max(a.y, b.y));
	}
	else
	{
		if (Distance(p, center) != Distance(a, center))
			return 0;
		double angleP = atan2(p.y - center.y, p.x - center.x);
		return AngleContains(angleP);
	}
}

bool Line::AngleContains(double angle)
{
	if (!type)
		throw "Stright line can't run AngleContains()";
	else
	{
		double stangle = atan2(a.y - center.y, a.x - center.x);
		double endangle = atan2(b.y - center.y, b.x - center.x);
		return AngleIsInRange(angle, stangle, endangle);
	}
}

Point Line::Projection(Point p)
{
	if (type)
		throw "arc can't use Projection()";
	else
	{
		double *para = Paramater();
		double A = para[0], B = para[1], C = para[2];
		double Px = p.x - A * (A * p.x + B * p.y + C) / (A * A + B * B);
		double Py = p.x - A * (A * p.x + B * p.y + C) / (A * A + B * B);
		return Point(Px, Py);
	}
}

double Line::Angle()
{
	if (type)
		throw "arc can't use Angle()";
	else
	{
		return atan2(b.y - a.y, b.x - a.x);
	}
}

double Line::StartAngle()
{
	if (!type)
		throw "stright line can't use StartAngle()";
	else
	{
		return atan2(a.y - center.y, a.x - center.x);
	}
}

double Line::EndAngle()
{
	if (!type)
		throw "stright line can't use EndAngle()";
	else
	{
		return atan2(b.y - center.y, b.x - center.x);
	}
}

Line Line::RelToAbs(Point origin)
{
	if (type)
		return Line(center.RelToAbs(origin), StartAngle(), EndAngle(), Radius());
	else
		return Line(a.RelToAbs(origin), b.RelToAbs(origin));
}

Line Line::AbsToRel(Point origin)
{
	if (type)
		return Line(center.AbsToRel(origin), StartAngle(), EndAngle(), Radius());
	else
		return Line(a.AbsToRel(origin), b.AbsToRel(origin));
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
			if (Distance(a.center, b.center) > a.Radius() + b.Radius())
			{
				return false;
			}
			double ang_ab = atan2(b.center.y - a.center.y, b.center.x - a.center.x);
			return a.AngleContains(ang_ab) && b.AngleContains(ang_ab + PI);
		}
		else
		{
			if (a.type)
			{
				swap(a, b);
			}
			// a为直线，b为圆弧
			if (Distance(a, b.center) > b.Radius())
				return 0;
			Line La(b.center, a.a), Lb(b.center, a.b); // 从圆心向两端点连线
			return AngleIntersects(La.Angle(), Lb.Angle(), b.StartAngle(), b.EndAngle());
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

void Shape::Draw(Point origin, COLORREF color)
{
	for (size_t i = 0; i < lines.size(); i++)
	{
		lines[i].Draw(origin, color);
	}
}

void Shape::Rotate(Point centerPoint, double angle)
{
	for (size_t i = 0; i < lines.size(); i++)
	{
		lines[i].Rotate(centerPoint, angle);
	}
}

Shape Shape::Zoom(Point centerPoint, double scale)
{
	Shape ret;
	for (size_t i = 0; i < lines.size(); i++)
	{
		ret.lines.push_back(lines[i].Zoom(centerPoint, scale));
	}
	return ret;
}

Shape Shape::RelToAbs(Point origin)
{
	Shape ret = *this;
	for (int i = 0; i < lines.size(); i++)
	{
		ret.lines[i].RelToAbs(origin);
	}
	return ret;
}

Shape Shape::AbsToRel(Point origin)
{
	Shape ret = *this;
	for (int i = 0; i < lines.size(); i++)
	{
		ret.lines[i].AbsToRel(origin);
	}
	return ret;
}

bool Crash(Shape a, Shape b)
{
	AABB A = a.GetAABB();
	AABB B = b.GetAABB();
	if (!Crash(A, B))
		return 0;
	else
	{
		for (int i = 0; i < a.lines.size(); i++)
		{
			for (int j = 0; j < b.lines.size(); j++)
			{
				if (Crash(a.lines[i], b.lines[j]))
					return 1;
			}
		}
		return 0;
	}
}

Shape Rectangle(Point center, int width, int height)
{
	vector<Line> lines;
	lines.push_back(Line(Point(center.x - width / 2, center.y - height / 2), Point(center.x + width / 2, center.y - height / 2)));
	lines.push_back(Line(Point(center.x + width / 2, center.y - height / 2), Point(center.x + width / 2, center.y + height / 2)));
	lines.push_back(Line(Point(center.x + width / 2, center.y + height / 2), Point(center.x - width / 2, center.y + height / 2)));
	lines.push_back(Line(Point(center.x - width / 2, center.y + height / 2), Point(center.x - width / 2, center.y - height / 2)));
	return Shape(lines);
}

Shape Circle(Point center, int r)
{
	vector<Line> lines;
	lines.push_back(Line(center, 0, 2 * PI, r));
	return Shape(lines);
}

Shape Squre(Point center, int width)
{
	return Rectangle(center, width, width);
}

Image::Image(string Address, int Height, int Width)
{
	address = Address;
	height = Height;
	width = Width;
	loadimage(&image, Address.c_str(), Height, Width, true);
	if (height * width == 0)
	{
		height = image.getheight();
		width = image.getwidth();
	}
}

void Image::SetSize(int Height, int Width)
{
	height = Height;
	width = Width;
}

void Image::Draw(Point origin, DWORD dwrop)
{
	Point pos = origin.ToEasyX();
	putimage(pos.x - (width / 2), pos.y - (height / 2), &image, dwrop);
}

void Image::Rotate(double angle)
{
	ang += angle;
}

void Image::SetAng(double angle)
{
	ang = angle;
}

void Image::Load()
{
	loadimage(&image, address.c_str(), height, width, true);
	rotateimage(&image, &image, ang, BLACK, true, true);
	// TODO:图片旋转后空白部分填充问题
}

void Image::SetAddress(string Address)
{
	address = Address;
}

Image Image::Zoom(double scale)
{
	return Image(address,height*scale,width*scale);
}

Entity::Entity()
{
	CrashBox = Shape();
	skinIndex = 0;
}

Entity::Entity(Image image)
{
	CrashBox = Rectangle(Point(0, 0), image.width, image.height);
	skins.push_back(image);
	skinIndex = 0;
}

void Entity::SetPosition(int x, int y)
{
	pos.Set(x, y);
}

void Entity::SetPosition(Point p)
{
	pos = p;
}

void Entity::SetCrashBox(Shape box)
{
	CrashBox = box;
}

Point Entity::GetPosition()
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
		skinIndex = index;
	}
}

void Entity::DelSkin(int index)
{
	if (index >= 0 && index < skins.size())
	{
		skins.erase(skins.begin() + index);
		if (index <= skinIndex)
			skinIndex--;
	}
}

int Entity::AddSkin(Image img)
{
	skins.push_back(img);
	return skins.size() - 1;
}

int Entity::AddSkin(string path)
{
	Image img(path);
	skins.push_back(img);
	if(skins.size() == 1)
		skinIndex = 0;
		//md这块空皮肤没处理我排了整整一天。
	return skins.size() - 1;
}

void Entity::Draw(Point origin)
{
	Point realPos = pos.RelToAbs(origin);
	CurrentSkin()->Draw(realPos);
}

void Entity::Move(int dx, int dy)
{
	pos.Move(dx, dy);
}

void Entity::MoveTo(Point newPos)
{
	pos = newPos;
}

void Entity::MoveTo(int x, int y)
{
	pos.Set(x, y);
}

Entity* Entity::Zoom(Point center, double scale)
{
	Entity *ret = new Entity();
	ret->CrashBox = this->CrashBox.Zoom(Point(0, 0), scale);
	ret->pos = this->pos.Zoom(center, scale);
	for (int i = 0; i < skins.size(); i++)
	{
		ret->AddSkin(this->skins[i].Zoom(scale));
		ret->skins[i].Load();
	}
	return ret;
}

void Entity::Rotate(Point origin, double arg)
{
	pos.Rotate(origin, arg);
	CrashBox.Rotate(Point(0, 0), arg);
	for (int i = 0; i < skins.size(); i++)
	{
		skins[i].Rotate(arg);
	}
}

Image *Entity::CurrentSkin()
{
	return &(skins[skinIndex]);
}

bool Crash(Entity a, Entity b)
{
	return Crash(a.CrashBox, b.CrashBox);
}

Scene::Scene(Point p)
{
	origin = p;
	maxPriority = 0;
}

void Scene::SetOrigin(Point p)
{
	origin = p;
}

int Scene::AddEntity(Entity *entity, int priority)
{
	pair<Entity *, int> p;
	p.first = entity;
	p.second = priority;
	entities.push_back(p);
	maxPriority = maxPriority > priority ? maxPriority : priority;
	return entities.size() - 1;
}

void Scene::DelEntity(int index)
{
	if (InRange(index, 0, entities.size() - 1))
	{
		entities[index] = entities.back();
		entities.pop_back();
		maxPriority = 0;
		for (int i = 0; i < entities.size(); i++)
		{
			maxPriority = max(maxPriority, entities[i].second);
		}
	}
}

void Scene::SetEntity(Entity *entity, int index)
{
	if (InRange(index, 0, entities.size() - 1))
		entities[index].first = entity;
}

int Scene::GetIndex(Entity *entity)
{
	for (int i = 0; i < entities.size(); i++)
	{
		if (entities[i].first == entity)
			return i;
	}
	return -1;
}

void Scene::SetPriority(int p, int index)
{
	if (InRange(index, 0, entities.size() - 1))
	{
		entities[index].second = p;
		maxPriority = maxPriority > p ? maxPriority : p;
	}
}

void Scene::Draw()
{
	BeginBatchDraw();
	setfillcolor(bgColor);
	fillrectangle(0, 0, GRAPH_WIDTH, GRAPH_HEIGHT);
	for (int i = 0; i <= maxPriority; i++)
	{
		for (int j = 0; j < entities.size(); j++)
		{
			if (entities[j].second == i)
			{
				/*
				Entity *e = (entities[j].first);
				e->Zoom(Point(0, 0), scale);
				e->Draw(origin);
				e->Zoom(Point(0, 0), 1.0/scale);
				*/
				entities[j].first->Zoom(Point(0,0),scale)->Draw(origin);
			}
		}
	}
	EndBatchDraw();
}

void Scene::SetScale(double Scale)
{
	scale = Scale;
}

void Scene::SetBackgroundColor(COLORREF color)
{
	bgColor = color;
}

Point Scene::GetMousePos()
{
	Point p = MousePos();
	p = p.AbsToRel(origin);
	p = p.Zoom(Point(0, 0), 1.0 / scale);
	return p;
}

void RefreshKeyState()
{
	for(int i = 0; i < 256; i++)
	{
		LastKeyState[i] = KeyState[i];
	}
	ExMessage msg;
	while (peekmessage(&msg, EM_KEY))
	{
		if (msg.message == WM_KEYDOWN)
		{
			KeyState[msg.vkcode] = KEY_DOWN;
		}
		else
		{
			KeyState[msg.vkcode] = KEY_UP;
		}
	}
}

bool KeyDown(char key)
{
	return KeyState[(unsigned char)key] == KEY_DOWN;
}

bool KeyUp(char key)
{
	return KeyState[(unsigned char)key] == KEY_UP;
}

bool KeyPress(char key)
{
	return KeyState[(unsigned char)key] == KEY_DOWN && LastKeyState[(unsigned char)key] == KEY_UP;
}

bool KeyRelease(char key)
{
	return KeyState[(unsigned char)key] == KEY_UP && LastKeyState[(unsigned char)key] == KEY_DOWN;
}

void Textbox::SetText(string Text)
{
	text = Text;
}

void Textbox::SetFont(int FontSize, string FontName)
{
	fontSize = FontSize;
	fontName = FontName;
}

void Textbox::SetColor(COLORREF Color)
{
	color = Color;
}

void Textbox::Draw(Point origin)
{
	//Entity::Draw(origin);
	settextcolor(color);
	settextstyle(int(fontSize), 0, fontName.c_str());
	Point realpos = pos.AbsToRel(origin);
	Point realleftup = Point(pos.x - width / 2, pos.y - height / 2);
	Point realrightdown = Point(pos.x + width / 2, pos.y + height / 2);
	RECT rect = {realleftup.x, realleftup.y, realrightdown.x, realrightdown.y};
	drawtext(text.c_str(), &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void Textbox::SetSize(int Width, int Height)
{
	width = Width;
	height = Height;
}

Textbox::Textbox():Entity()
{
	text = "";
	fontSize = 0;
	fontName = "";
	color = WHITE;
	width = 0;
	height = 0;
}

Textbox::Textbox(string Text, int Width, int Height, int FontSize, string FontName, COLORREF Color) :Entity()
{
	text = Text;
	width = Width;
	height = Height;
	CrashBox = Rectangle(Point(0,0),Width,Height);
	color = Color;
	fontSize = FontSize;
	fontName = FontName;
}

AABB AABB::Zoom(Point center,double scale)
{
	Point MinP(minX,minY);
	Point MaxP(maxX,maxY);
	MinP = MinP.Zoom(center,scale);
	MaxP = MaxP.Zoom(center,scale);
	return AABB(MinP.x,MaxP.x,MinP.y,MaxP.y);
}

Textbox* Textbox::Zoom(Point center,double scale)
{
	Textbox* ret = new Textbox();
	ret->CrashBox = this->CrashBox.Zoom(Point(0, 0), scale);
	ret->pos = this->pos.Zoom(center, scale);
	for (int i = 0; i < skins.size(); i++)
	{
		ret->AddSkin(this->skins[i].Zoom(scale));
		ret->skins[i].Load();
	}
	ret->fontSize *= scale;
	ret->width *= scale;
	ret->height *= scale;
	return ret;
}

void Entity::Debug()
{
	cout<<"Pos:("<<pos.x<<","<<pos.y<<")\n";
}