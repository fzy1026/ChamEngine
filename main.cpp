#include "lib/chamtool.h"
#include "lib/chamgame.h"
#include <vector>
#include <iostream>

using namespace std;
Point SQPUL;
Point SQPDR;

Shape SkillIcon(int kind)
{
	Shape res;
	res.SetCenter(0, 0);

	switch (kind)
	{
	case 1://Ligntning
		res.p.push_back(Point(-1.5, 3.5));
		res.p.push_back(Point(-0.5, 0.5));
		res.p.push_back(Point(-1.5, -0.5));
		res.p.push_back(Point(1.5, -3.5));
		res.p.push_back(Point(0.5, -0.5));
		res.p.push_back(Point(1.5, 0.5));
		res.SetPolygon(YELLOW);
		res = Zoom(res, 5);
	}
	return res;
}

class SkillBox : public Object {
	public:
		int kind;
		SkillBox(int k)
		{
			kind = k;
			Shape CP;
			CP.SetCenter(0, 0);
			CP.SetRectangle(Point(-20, -20), YELLOW);
			crashbox = CP;
			AddSkin(SkillIcon(kind) + CP);
		}
};

Shape TankIcon(COLORREF C)
{
	Point Ct(20, 20);
	Shape s1;
	int w, h;
	w = 20;
	h = 50;
	s1.SetCenter(Ct);
	s1.line_color = C;
	s1.p.push_back(Ct.Move(-w / 2.0, -h / 2.0));
	s1.p.push_back(Ct.Move(w / 2.0, -h / 2.0));
	s1.p.push_back(Ct.Move(w / 2.0, h / 2.0));
	s1.p.push_back(Ct.Move(-w / 2.0, h / 2.0));
	s1.SetPolygon(C);
	s1.SetCenter(Ct.Move(0, h / 4.0));
	Shape s2;
	s2.SetCenter(Ct);
	s2.SetCircle(30, C);
	Shape s3;
	s3 = s1 + s2;
	return s3;
}
Shape BulletIcon()
{
	Shape ans1, ans2;
	ans1.SetCenter(Point(50, 40));
	ans1.p.push_back(Point(40, 0));
	ans1.p.push_back(Point(40, 60));
	ans1.p.push_back(Point(50, 100));
	ans1.p.push_back(Point(60, 60));
	ans1.p.push_back(Point(60, 0));
	ans1.SetPolygon();
	ans2.SetCenter(Point(50, 40));
	ans2.p.push_back(Point(40, 0));
	ans2.p.push_back(Point(40, 10));
	ans2.p.push_back(Point(60, 10));
	ans2.p.push_back(Point(60, 0));
	ans2.SetPolygon();
	ans1 = ans1 + ans2;
	ans1 = Rotate(ans1, PI / 4.0 + 2 * PI / 3.0);
	return ans1;
}
class Bullet: public Object {
	public:
		bool exist;
		bool cb;
		double speed;
		vector<bool> last_ce;
		int life;
		Bullet()
		{
			Shape s;
			last_ce.push_back(0);
			last_ce.push_back(0);
			last_ce.push_back(0);
			last_ce.push_back(0);
			last_ce.push_back(0);
			Point Ct(200, 200);
			s.SetCenter(Ct);
			s.SetCircle(6, WHITE);
			speed = 16;
			exist = 1;
			life = 5;
			SetCenter(Ct);
			AddSkin(s);
			crashbox = s;
			ang = 0;
		}
};

vector<Bullet> bullet;
class Tank: public Object {
	public:
		Image tank_image;
		Image died_image;
		COLORREF tank_color;
		double speed, aspeed;
		int bullet_num, max_bullet_num;
		int reload_cd;
		int last_add_time;
		char key_up, key_down, key_left, key_right, key_shoot;
		Tank(COLORREF C = WHITE)
		{
			SetCenter(Point(20, 20));
			tank_color = C;
			now_skin = -1;
			max_bullet_num = 5;
			reload_cd = 1;
			bullet_num = 5;
			last_add_time = -100;
			AddSkin(TankIcon(C));
			Shape BT;
			BT = TankIcon(C);
			Shape BL;
			BL.SetCenter(Point(20, 20));
			speed = 15;
			aspeed = PI / 45;
			Shape s2;
			s2.SetCenter(Point(20, 20));
			s2.SetCircle(30, C);
			crashbox = s2;

			for (int i = 1; i <= 20; i++)
			{
				BL.phy_l.push_back(Line(BL.center, Polor(i * PI / 10.0, 30, BL.center), RED));
			}

			BT = BL + BT;

			AddSkin(BT);
			ang = PI / 2.0;
		}

		void Draw()
		{
			if (now_skin < 0)
				return;

			skin[now_skin].Draw();
			tank_image.pos = center.Move(30, 30);
			tank_image.Draw();
			died_image.pos = center.Move(30, 30);
		}

		void SetTankColor(COLORREF CC)
		{
			tank_color = CC;
			now_skin = -1;
			max_bullet_num = 5;
			reload_cd = 1;
			bullet_num = 5;
			last_add_time = -100;
			speed = 4;
			SetCenter(Point(200, 200));
			aspeed = PI / 40.0;
			AddSkin(TankIcon(CC));
			Shape BT = TankIcon(CC);
			Shape BL;
			BL.SetCenter(Point(20, 20));

			for (int i = 1; i <= 20; i++)
			{
				BL.phy_l.push_back(Line(BL.center, Polor(i * PI / 10.0, 15, BL.center), RED));
			}

			BT = BL + BT;
			AddSkin(BT);
			Shape s2;
			s2.SetCenter(Point(20, 20));
			s2.SetCircle(15, CC);
			crashbox = s2;
			ang = PI / 2.0;
		}

		Bullet Run(Tank T)
		{
			Point ShootP = Polor(ang, 40, center);
			Bullet new_b;
			new_b.MoveTo(ShootP.x, ShootP.y);
			new_b.ang = ang;
			new_b.exist = 0;

			if (KeyDown(key_up))
			{
//				only_one = 0;
				Go(speed);

				if (CrashEdge(crashbox, SQPUL, SQPDR)[0] || Crash(*this,T))
				{
					Go(-speed);
//					only_one = 1;
				}
			}

			if (KeyDown(key_down))
			{
//				only_one = 0;
				Go(-speed);

				if (CrashEdge(crashbox, SQPUL, SQPDR)[0])
				{
					Go(speed);
//					only_one = 1;
				}
			}

			if (KeyDown(key_left))
			{
//				only_one = 0;
				SelfRotate(aspeed);
			}

			if (KeyDown(key_right))
			{
//				only_one = 1;
				SelfRotate(-aspeed);
			}

			if (time(0) - last_add_time > reload_cd && bullet_num < max_bullet_num)
			{
				bullet_num++;
				last_add_time = time(0);
			}

			if (bullet_num == max_bullet_num)
			{
				last_add_time = time(0);
			}

			if (KeyClick(key_shoot) && bullet_num > 0)
			{

				if (CrashEdge(new_b, SQPUL, SQPDR)[0])
				{
					new_b.exist = 0;
				}
				else
				{
					bullet_num--;
					new_b.exist = 1;
				}
			}

			return new_b;
		}
		void SetKey(int w, int s, int a, int d, int xx)
		{
			key_up = w;
			key_down = s;
			key_left = a;
			key_right = d;
			key_shoot = xx;
		}
};
vector<Tank> tk;

void Init()
{
	srand(time(0));
	SetFPS(120);
	EngineInit(1280, 960, EX_SHOWCONSOLE);
	cout << "Init() done\n";
	ClearScreen();
	cout << "Screen cleared.\n";
}
void WelcomeMenu()
{
	Button StartGame("TANK FIGHT");
	StartGame.SetCenter(Point(WinWidth / 2, WinHeight / 2));
	StartGame.crashbox.SetRectangle(Point(WinWidth / 2 + 200, WinHeight / 2 + 60));
	StartGame.SetTextRec(Point(WinWidth / 2 + 200, WinHeight / 2 + 60));
	StartGame.AddSkin(StartGame.crashbox);
	StartGame.Draw();

	while (1)
	{
		time_t tick_start = clock();
		if (StartGame.Click())
		{
			break;
		}

		time_t tick_now = (1e6 / FPS) - (clock() - tick_start);
		if (tick_now < 0)
		{
			cout << "Error!Setting FPS is too high\n";
			tick_now = 0;
		}
		usleep(tick_now);
	}
}
void GameReset()
{
	for (int i = 0; i <= 1; i++)
	{
		tk[i].MoveTo(Rand(SQPUL.x + 40, SQPDR.x - 40), Rand(SQPUL.y + 40, SQPDR.y - 40));
		tk[i].bullet_num = 5;
	}

	bullet.clear();
}

void StartGame()
{
	Tank tk1(YELLOW);
	tk1.tank_image.SetAddress("tk1.png");
	tk1.tank_image.LoadFile(80, 80);
	tk1.died_image.SetAddress("died.png");
	tk1.died_image.LoadFile(80, 80);
	tk1.SetKey('W', 'S', 'A', 'D', ' ');

	Tank tk2(GREEN);
	tk2.SetKey(VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT, 'L');
	tk2.tank_image.SetAddress("tk2.png");
	tk2.tank_image.LoadFile(80, 80);
	tk2.died_image.SetAddress("died.png");
	tk2.died_image.LoadFile(80, 80);

	tk.push_back(tk1);
	tk.push_back(tk2);
	cout << "Tanks load successfully!\n";
	SQPUL = Point(0, 0);
	SQPDR = Point(1280, 760);
	GameReset();

	while (1)
	{
		time_t tick_start = clock();
		ClearScreen();
		int Res = 0;

		bool cycle = 1;

		if (cycle)
		{
			cycle = 0;

			for (int i = 0; i < int(bullet.size()); i++)
			{
				if (bullet[i].exist == 0)
				{
					bullet.erase(bullet.begin() + i);
					cycle = 1;
					break;
				}
			}
		}


		//UI
		Shape BI = BulletIcon();

		for (int j = 0; j <= 1; j++)
		{
			for (int i = 0; i < tk[j].bullet_num; i++)
			{
				BI.MoveTo(25 + 30 * i + j * WinWidth / 2, 900);
				BI.Draw();
			}
		}

		for (int i = 0; i < int(tk.size()); i++)
		{
			Bullet addB = tk[i].Run(tk[(i+1)%tk.size()]);

			if (addB.exist)
			{
				bullet.push_back(addB);
			}
		}

		Shape TK1 = TankIcon(tk[0].tank_color);
		Shape TK2 = TankIcon(tk[1].tank_color);
		TK1.MoveTo(WinWidth / 2 - 50, WinHeight - 40);
		TK2.MoveTo(WinWidth - 50, WinHeight - 40);
		TK1.Draw();
		TK2.Draw();
		Line l1(Point(WinWidth / 2, SQPDR.y), Point(WinWidth / 2, WinHeight));
		l1.Draw();
		rectangle(SQPUL.x, SQPUL.y, SQPDR.x, SQPDR.y);

		for (int i = 0; i <= 1; i++)
		{
			tk[i].Draw();
		}

		for (int i = 0; i < int(bullet.size()); i++)
		{
			bullet[i].Go(bullet[i].speed);
			vector<bool> ce = CrashEdge(bullet[i], SQPUL, SQPDR);

			if (Crash(bullet[i], tk[0]))
			{
				Res = 1;
			}

			if (Crash(bullet[i], tk[1]))
			{
				Res = 2;
			}

			if (!ce[0])
			{

			}
			else
			{
				if (ce[1])
				{
					bullet[i].ang = Rebound(bullet[i].ang, PI / 2);
					bullet[i].life--;
				}

				if (ce[2])
				{
					bullet[i].ang = Rebound(bullet[i].ang, 0);
					bullet[i].life--;
				}

				if (ce[3])
				{
					bullet[i].ang = Rebound(bullet[i].ang, PI / 2);
					bullet[i].life--;
				}

				if (ce[4])
				{
					bullet[i].ang = Rebound(bullet[i].ang, 0);
					bullet[i].life--;
				}

				bullet[i].Go(bullet[i].speed);
			}

			if (bullet[i].life <= 0)
			{
				bullet[i].exist = 0;
			}

			bullet[i].Draw();
		}

		FlushBatchDraw();

		if (Res != 0)
		{
			cout << "P" << Res << " Game over!\n";
			tk[Res - 1].NextSkin();
			tk[Res - 1].Draw();
			tk[Res - 1].died_image.Draw();
			FlushBatchDraw();
			sleep(3);
			tk[Res - 1].NextSkin();
			GameReset();
		}

		time_t tick_now = (1e6 / FPS) - (clock() - tick_start);
		if (tick_now < 0)
		{
			cout << "Error!Setting FPS is too high\n";
			tick_now = 0;
		}
		usleep(tick_now);
	}
}

void Debug()
{
	cout << "Debug Start\n";
	cout << "Debug End\n";
}


int main()
{
	Init();
	cout << "init done.\n";
	Debug();
	WelcomeMenu();
	cout << "Welcome!\n";
//	ExMessage msg;
//	int Res = 0;
	BeginBatchDraw();
	StartGame();
	return 0;
}
