#include "include/chamgame.h"
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;
//TODO:基础demo
const int WinWidth = 640,WinHeight = 480;


int main()
{
	Point Origin(WinWidth/2,WinHeight/2);
	InitEngine(WinWidth, WinHeight);
	Textbox textbox("Hello, World!", 600, 100);
	textbox.SetPosition(0, 0);
	textbox.AddSkin("resources/images/test.png");
	//textbox.SetSize(200,200);
	textbox.SetFont(50,"微软雅黑");

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
