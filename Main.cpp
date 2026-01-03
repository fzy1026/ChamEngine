#include "include/chamgame.h"
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;


int main() {
	initgraph(640, 480);
	Entity entity;
	entity.SetPosition(100, 100);
	entity.SetSkin(entity.AddSkin("resources\\images\\test.png")); 
	//putimage(320,240,&(entity.skins[0].image));
	
	Scene main(Point(320,240));
	main.AddEntity(&entity);
	main.SetScale(0.25);
	main.Draw();
	main.origin.Draw(Point(0,320),RED);
	// 按任意键退出
	system("pause");
	closegraph();

	return 0;

}
