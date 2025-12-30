#include "include/chamtool.h"
#include "include/chamgame.h"
#include <vector>
#include <iostream>

using namespace std;


int main() {
	initgraph(640, 480);
	Point o(0,480);
	Entity entity;
	entity.SetPosition(100, 100);
	entity.SetSkin(entity.AddSkin("resources\\images\\test.png"));
	entity.Draw(Point(0, 480));

	// 按任意键退出
	getch();
	closegraph();

	return 0;

}
