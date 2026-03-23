#include "include/chamgame.h"
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;
//TODO:基础demo
const int WinWidth = 640,WinHeight = 480;

class Bullet : public Entity
{
public:
	Bullet(Point pos)
	{
		Entity();
		this->pos = pos;
		Image img("res/images/bullet.png");
		AddSkin(img);
		crashbox = Rectangle(Point(0, 0), img.width, img.height);
		skinIndex = 0;
	}

	void Run() override
	{
		cout<<"Bullet run"<<endl;
		Move(0, 10);
		if(pos.y > WinHeight/2)
		{
			//cout<<"Bullet deleted by out of bound"<<endl;
			delete this;
		}
	}

	~Bullet()
	{
		//cout<<"Bullet deleted"<<endl;
	}
};

void OriginDraw(Image img, Point origin)
{
	img.Load();
	Point pos = origin.ToEasyX();
	putimage(pos.x, pos.y, &(img.image));
}


int main()
{
	Point Origin(WinWidth/2,WinHeight/2);
	InitEngine(WinWidth, WinHeight);
	
	Image TestImg("res/images/half_red.png");
	TestImg.Load();
	TestImg.Draw(Origin);
	system("pause");
	
	OriginDraw(TestImg, Origin);
	system("pause");
	

	Entity TestEntity;
	TestEntity.SetPosition(0, 0);
	TestEntity.AddSkin("res/images/test.png");
	Entity plane;
	plane.AddSkin("res/images/plane.png");
	plane.SetPosition(0,0);

	Scene main(Origin);
	COLORREF bgColor = BLACK;
	main.SetBackgroundColor(bgColor);
	main.scale = 0.25;

	main.AddEntity(&plane,1);
	main.AddEntity(&TestEntity,0);



	while (1)
	{
		
		RefreshKeyState();
		
		
		if(KeyDown('W'))
		{
			//cout<<'W'<<endl;
			plane.Move(0,10);
		}
		if(KeyDown('S'))
		{
			//cout<<'S'<<endl;
			plane.Move(0,-10);
		}
		if(KeyDown('A'))
		{
			//cout<<'A'<<endl;
			plane.Move(-10,0);
		}
		if(KeyDown('D'))
		{
			//cout<<'D'<<endl;
			plane.Move(10,0);
		}
			
		if(KeyDown('X'))
		{
			//cout<<'X'<<endl;
			break;
		}

		if(KeyPress('J'))
		{
			Bullet* bullet = new Bullet(plane.GetPosition());
			main.AddEntity(bullet);
		}
		

		main.Draw();

	}
	closegraph();
	

	return 0;
}
