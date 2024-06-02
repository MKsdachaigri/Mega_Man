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

	Player(Texture& image) {
		sprite.setTexture(image);
		rect = FloatRect(1 * ts, 8 * ts, 42, 48);
		sprite.setTextureRect(IntRect(56, 8, 26, 26));
		sprite.setScale(2.0, 2.0);

		dx = dy = 0;
		curFrame = 0;
		rig = 1;
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

		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);

			sprite.move(0.2 * time, 0);
			if (dx > 0)
			{
				sprite.setTextureRect(IntRect(56, 8, 26, 26));
				rig = 1;
			}
			if (dx < 0)
			{
				sprite.setTextureRect(IntRect(56 + 26, 8, -26, 26));
				rig = 0;
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

int main()
{
	RenderWindow window(VideoMode(1000, 500), "Mega Man");
	Texture Picture;
	Picture.loadFromFile("C:/random_game_files/finalsprites.png");
	Player p (Picture);

	Texture t1;
	t1.loadFromFile("C:/random_game_files/bullet.png");
	Bullet bul(t1);

	Texture t2;
	t2.loadFromFile("C:/random_game_files/platforms.png");
	Sprite platform(t2);

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
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			p.dx = -0.12;
		}
		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			p.dx = 0.12;
		}
		if (Keyboard::isKeyPressed(Keyboard::X))
		{
			if (p.onGround)
			{
				p.dy = -0.3;
				p.onGround = 0;
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Z))
		{
			if (bul.go == 0)
			{
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



		if(p.rect.left > 500 && p.rect.left < 1000)
		offsetX = p.rect.left - 500;
		if(p.rect.top > 500)
		offsetY = p.rect.top - 250;

		p.update(time);
		bul.update(time);
		window.clear(Color::White);

		for (int i = 0; i < H; i++)
		{
			for (int j = 0; j < W; j++)
			{
				if (Map[i][j] == 'A')
				{
					platform.setTextureRect(IntRect(0, 0, ts, ts));
				}
				if (Map[i][j] == ' ') continue;
				platform.setPosition(j * ts - offsetX, i * ts - offsetY);
				window.draw(platform);
			}
		}

		window.draw(p.sprite);
		if (bul.go != 0)
		{
			window.draw(bul.sprite);
		}
		window.display();
	}
	return 0;
	//commment
}