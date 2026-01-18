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
	main.scale = 0.25;
	main.AddEntity(&entity);
	while (1)
	{
		entity.MoveTo(main.GetMousePos());
		main.Draw();
	}

	// 按任意键退出
	system("pause");
	closegraph();

	return 0;
}
