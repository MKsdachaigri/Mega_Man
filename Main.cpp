#include <iostream>
#include <conio.h>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

float offsetX = 0, offsetY = 0;

const int H = 10;
const int W = 30;

const int ts = 50;

String Map[H] = {
	"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
	"A                            A",
	"A                            A",
	"A                            A",
	"A           AAAA             A",
	"A          A                 A",
	"A       A                    A",
	"A     AAAA                   A",
	"A    AAAAA                   A",
	"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" };



class Player {
public:
	float dx, dy;
	FloatRect rect;
	bool onGround, rig;
	Sprite sprite;
	float curFrame;
	bool life;
	int hp;

	Player(Texture& image) {
		sprite.setTexture(image);
		rect = FloatRect(1 * ts, 8 * ts, 52, 47);
		sprite.setTextureRect(IntRect(77, 0, 77, 65));
		sprite.setScale(0.75, 0.75);

		dx = dy = 0;
		curFrame = 0;
		rig = 1;
		life = 1;
		hp = 3;
	}

	void update(float time) {
		rect.left += dx * time;
		Collision(0);
		if (!onGround)
		{
			dy = dy + 0.0005 * time;
		}
		rect.top += dy * time;
		onGround = 0;
		Collision(1);
		if (life)
		{
			if (hp == 0)
			{
				life = 0;
			}
			curFrame += 0.01 * time;
			if (curFrame > 9)
				curFrame -= 8;
			sprite.setPosition(rect.left - offsetX, rect.top - offsetY);

			sprite.move(0.2 * time, 0);
			if (dx > 0)
			{
				if (dy == 0)
				{
					sprite.setTextureRect(IntRect(77 * int(curFrame), 0, 77, 65));
				}
				else
				{
					sprite.setTextureRect(IntRect(0, 81, 45, 65));
				}
				rig = 1;
			}
			if (dx < 0)
			{
				if (dy == 0) 
				{
					sprite.setTextureRect(IntRect(77 * int(curFrame) + 77, 0, -77, 65));
				}
				else 
				{
					sprite.setTextureRect(IntRect(45, 81, -45, 65));
				}
					rig = 0;
			}
		}
		else
		{
			if (rig)
			{
				sprite.setTextureRect(IntRect(156, 104, 82, 30));
			}
			else
			{
				sprite.setTextureRect(IntRect(238, 104, -82, 30));
			}
		}
			sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
			dx = 0;
		
	}
	void Collision(float dir)
	{
		for (int i = rect.top/ts; i < (rect.top+rect.height)/ts; i++)
		{
			for (int j = rect.left/ts; j < (rect.left + rect.width) / ts; j++)
			{
				if (Map[i][j] == 'A')
				{
					if (dx > 0 && dir == 0)
					{
						rect.left = j * ts - rect.width;
					}
					if (dx < 0 && dir == 0)
					{
						rect.left = j * ts + ts;
					}
					if (dy > 0 && dir == 1)
					{
						rect.top = i * ts - rect.height;
						dy = 0;
						onGround = 1;
					}
					if (dy < 0 && dir == 1)
					{
						rect.top = i * ts +ts;
						dy = 0;
					}
				}
			}
		}
	}
};

class Bullet {
public:
	float dx, dy;
	FloatRect rect;
	int go;
	Sprite sprite;

	Bullet(Texture& image)
	{
		sprite.setTexture(image);
		dx = dy = 0;
		go = 0;
	}
	void update(float time)
	{
		rect.left += dx * time;
		Collision(0);

		rect.top += dy * time;
		Collision(1);

		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
	}
	void Collision(int dir)
	{
		for (int i = rect.top / ts; i < (rect.top + rect.height) / ts; i++)
		{
			for (int j = rect.left / ts; j < (rect.left + rect.width) / ts; j++)
			{
				if (Map[i][j] == 'A')
				{
					if ((dx > 0 && dir == 0) || (dx < 0 && dir == 0))
					{
						go = 0;
					}
				}
			}
		}
	}
};

class Enemy {
public:
	float dx, dy;
	FloatRect rect;
	Sprite sprite;
	float curframe;
	bool life;

	void set(Texture& image, int x, int y)
	{
		sprite.setTexture(image);
		sprite.setScale(2.5, 2.5);
		rect = FloatRect(x, y, ts, ts);
		curframe = 0;
		dx = 0.1;
		life = 1;
	}

	void update(float time) {
		rect.left += dx * time;

		Collision();
		if (life)
		{
			curframe += time * 0.0015;
			if (curframe > 2)
			{
				curframe-= 2;
			}
			if (dx > 0)
			{
				sprite.setTextureRect(IntRect(20 * int(curframe), 0, 20, 19));
			}
			else if (dx < 0)
			{
				sprite.setTextureRect(IntRect(20*int(curframe) + 20, 0, -20, 19));
			}
		}
		else
		{
			sprite.setTextureRect(IntRect(0, 0, 0, 0));
		}
		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
	}


	void Collision() {
		for (int i = rect.top / ts; i < (rect.top + rect.height) / ts; i++)
			for (int j = rect.left / ts; j < (rect.left + rect.width) / ts; j++)
				if (Map[i][j] == 'A')
				{
					if (dx > 0)
					{
						rect.left = j * ts - rect.width;
						dx *= -1;
					}
					else if (dx < 0)
					{
						rect.left = j * ts + ts;
						dx *= -1;
					}
				}
	}
};

int main()
{
	RenderWindow window(VideoMode(1000, 500), "Mega Man");
	Texture Picture;
	Picture.loadFromFile("C:/random_game_files/mega_man_sprites.png");
	Player p (Picture);
	int frame = 0;

	Texture t1;
	t1.loadFromFile("C:/random_game_files/bullet.png");
	Bullet bul(t1);

	Texture t2;
	t2.loadFromFile("C:/random_game_files/platforms.png");
	Sprite platform(t2);

	Texture t3;
	t3.loadFromFile("C:/random_game_files/new_enemy.png");
	Enemy enemy;
	enemy.set(t3, 10 * ts,  8* ts);

	Texture t4;
	t4.loadFromFile("C:/random_game_files/health_bar.png");
	Sprite hp[4];
	for (int i = 0; i < 4; i++) {
		hp[i].setTexture(t4);
		hp[i].setScale(1.5, 2.0);
		if (i == 3)
		{
			hp[i].setTextureRect(IntRect(1, 265, 7, 56));
		}
		else if (i == 2)
		{
			hp[i].setTextureRect(IntRect(72, 265, 7, 56));
		}
		else if (i == 1)
		{
			hp[i].setTextureRect(IntRect(171, 265, 7, 56));
		}
		else if (i == 0)
		{
			hp[i].setTextureRect(IntRect(252, 265, 7, 56));
		}
		hp[i].setPosition(75, 75);
	}

	Clock clock;
	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 800;
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
			}
		}
		if (!_kbhit())
			if (p.dy == 0) {
				if (p.rig)
					p.sprite.setTextureRect(IntRect(0, 0, 77, 65));
				else if (!p.rig)
					p.sprite.setTextureRect(IntRect(77, 0, -77, 65));
			}
		if (p.life == 1)
		{
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				p.dx = -0.135;
			}
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				p.dx = 0.135;
			}
			if (Keyboard::isKeyPressed(Keyboard::X))
			{
				if (p.onGround)
				{
					p.dy = -0.3;
					p.onGround = 0;
					if (p.rig)
					{
						p.sprite.setTextureRect(IntRect(0, 81, 45, 65));
					}
					else if (!p.rig)
					{
						p.sprite.setTextureRect(IntRect(45, 81, -45, 65));
					}
				}
			}
			if (Keyboard::isKeyPressed(Keyboard::Z))
			{
				if (bul.go == 0 && p.onGround)
				{
					frame = 100;
					if (p.rig)
					{
						bul.go = 1;
						bul.rect.left = p.rect.left + 67;
						bul.rect.top = p.rect.top + 20;
					}
					else
					{
						bul.go = 2;
						bul.rect.left = p.rect.left - 12;
						bul.rect.top = p.rect.top + 18;

					}
				}
			}
		}
		if (bul.go == 0)
		{
			bul.rect.left = p.rect.left;
			bul.rect.top = p.rect.top;
		}
		else if (bul.go == 1)
		{
			bul.dx = 1;
		}
		else if (bul.go == 2)
		{
			bul.dx = -1;
		}

		if (frame != 0)
		{
			if (p.dy == 0)
			{
				if (p.rig)
				{
					p.sprite.setTextureRect(IntRect(68, 80, 69, 61));
				}
				else if (!p.rig)
				{
					p.sprite.setTextureRect(IntRect(68 + 69, 80, -69, 61));
				}
				frame--;
			}
		}
		if (enemy.life == 1)
		{
			if (p.rect.intersects(enemy.rect))
			{
				p.hp--;
				if (p.hp > 0)
				{
					if(p.rig > 0)
					{
						p.rect.left -= 100;
					}
					else
					{
						p.rect.left += 100;
					}
				}
			}
			if (enemy.rect.left < bul.rect.left && enemy.rect.left + 10 > bul.rect.left &&
				enemy.rect.top < bul.rect.top && enemy.rect.top + 50 > bul.rect.top && bul.go != 0)
			{
				enemy.life = 0;
				bul.go = 0;
			}
		}

		if(p.rect.left > 500 && p.rect.left < 1000)
		offsetX = p.rect.left - 500;
		if(p.rect.top > 500)
		offsetY = p.rect.top - 250;

		p.update(time);
		bul.update(time);
		enemy.update(time);
		window.clear(Color::White);

		for (int i = 0; i < H; i++)
		{
			for (int j = 0; j < W; j++)
			{
				if (Map[i][j] == 'A')
				{
					platform.setTextureRect(IntRect(50, 0, ts-1, ts));
					platform.setScale(1.045, 1.0);
				}
				if (Map[i][j] == ' ') continue;
				platform.setPosition(j * ts - offsetX, i * ts - offsetY);
				window.draw(platform);
			}
		}

		window.draw(p.sprite);
		window.draw(enemy.sprite);
		if (bul.go != 0)
		{
			window.draw(bul.sprite);
		}
		if (p.hp > 0) 
		{
			for (int i = 0; i <= p.hp; i++)
			{
				window.draw(hp[i]);
			}
		}
		else
		{
			window.draw(hp[0]);
		}
		window.display();
	}
	return 0;
	//commment
}