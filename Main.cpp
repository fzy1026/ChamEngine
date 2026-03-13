#include "include/chamgame.h"
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;
//TODO:文字展示
//TODO:基础demo


int main()
{
	Point Origin(320,240);
	InitEngine(640, 480);
	
	Textbox textbox("Hello, World!", 200, 100);
	textbox.SetPosition(0, 0);
	textbox.Draw(Origin);
	textbox.AddSkin("resources/images/test.png");

	Scene main(Origin);
	COLORREF bgColor = BLACK;
	main.SetBackgroundColor(bgColor);
	main.scale = 0.25;
	main.AddEntity(&textbox);

	while (1)
	{
		RefreshKeyState();
		if(KeyDown('W'))
		{
			textbox.Move(0,10);
		}
		if(KeyDown('S'))
		{
			textbox.Move(0,-10);
		}
		if(KeyDown('A'))
		{
			textbox.Move(-10,0);
		}
		if(KeyDown('D'))
		{
			textbox.Move(10,0);
		}
		if(KeyDown('X'))
		{
			break;
		}
		main.Draw();
	}
	closegraph();

	return 0;
}
