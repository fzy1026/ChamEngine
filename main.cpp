#include "include/chamtool.h"
#include "include/chamgame.h"
#include <vector>
#include <iostream>

using namespace std;


int main() {
	initgraph(640, 480);

	// 定义图像
	IMAGE img1, img2;
	
	// 从文件加载图像
	loadimage(&img1, _T("test.png"),100,100,1);
	// 旋转图像 30 度 (PI / 6)
	rotateimage(&img2, &img1, PI / 4,BLACK,true,true);
	
	// 显示旋转后的图像
	putimage(320, 240, &img2);

	// 按任意键退出
	getch();
	closegraph();

	return 0;

}
