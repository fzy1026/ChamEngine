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
	Point Origin(640,480);
	InitEngine(1280, 960);
	
	Textbox textbox("Hello, World!", 200, 100);
	textbox.SetPosition(0, 0);
	textbox.AddSkin("resources/images/test.png");
	textbox.SetSize(100,100);
	textbox.SetFont(40,"微软雅黑");

	Scene main(Origin);
	COLORREF bgColor = BLACK;
	main.SetBackgroundColor(bgColor);
	main.scale = 0.25;
	main.AddEntity(&textbox);

	int TestX = 10;
	textbox.Move(0,30);

	while (1)
	{
		
		RefreshKeyState();
		
		
		if(KeyDown('W'))
		{
			//cout<<'W'<<endl;
			textbox.Move(0,10);
		}
		if(KeyDown('S'))
		{
			//cout<<'S'<<endl;
			textbox.Move(0,-10);
		}
		if(KeyDown('A'))
		{
			//cout<<'A'<<endl;
			textbox.Move(-10,0);
		}
		if(KeyDown('D'))
		{
			//cout<<'D'<<endl;
			textbox.Move(10,0);
		}
			
		if(KeyDown('X'))
		{
			//cout<<'X'<<endl;
			break;
		}
		
		//textbox.Move(0,30);
		/*
		if(abs(textbox.pos.x) >= 100)
			TestX = -TestX;
		textbox.Move(TestX,0);
		*/
		

		main.Draw();
		//textbox.Debug();
	}
	closegraph();
	

	return 0;
}
