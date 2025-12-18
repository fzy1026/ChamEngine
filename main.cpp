#include "lib/chamtool.h"
#include "lib/chamgame.h"
#include <vector>
#include <iostream>

using namespace std;


int main()
{
	initgraph(800, 600);
	Position origin(0,600);
	double step = PI/18;
	
	for(double i=0;i <= 2*PI;i+=step)
	{
		for(double j=i+step;j <= 2*PI;j+=step)
		{
			Line arc(Point(400,300),i,j,100);
			arc.Draw(origin, WHITE);
			arc.GetAABB().Draw(origin, RED);
			Line(arc.a,arc.b).Draw(origin, GREEN);
			usleep(10000);
			clearrectangle(0,0,800,600);
		}
	}
	closegraph();
	return 0;

}
