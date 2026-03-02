#include "include/chamgame.h"
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;



int main()
{
	InitEngine(640, 480);
	Entity entity;
	entity.SetPosition(0, 0);
	entity.SetSkin(entity.AddSkin("resources\\images\\test.png"));

	Scene main(Point(320, 240));
	COLORREF bgColor = WHITE;
	main.SetBackgroundColor(bgColor);
	main.scale = 0.25;
	main.AddEntity(&entity);
	while (1)
	{
		RefreshKeyState();
		if(KeyDown('W'))
		{
			entity.Move(0,10);
		}
		if(KeyDown('S'))
		{
			entity.Move(0,-10);
		}
		if(KeyPress('A'))
		{
			entity.Move(-10,0);
		}
		if(KeyPress('D'))
		{
			entity.Move(10,0);
		}
		main.Draw();
	}

	// 按任意键退出
	system("pause");
	closegraph();

	return 0;
}
