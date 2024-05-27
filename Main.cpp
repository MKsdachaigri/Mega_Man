#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

float offsetX = 0, offsteY = 0;

const int H = 10;
const int W = 20;

const int ts = 50;

String Map[H] = {
	"AAAAAAAAAAAAAAAAAAAA",
	"A                  A",
	"A                  A",
	"A                  A",
	"A                  A",
	"A                  A",
	"A       A          A",
	"A     AAAA         A",
	"A    AAAA A        A",
	"AAAAAAAAAAAAAAAAAAAA" };

int ground = 400;

class Player {
public:
	float dx, dy;
	FloatRect rect;
	bool onGround;
	Sprite sprite;
	float curFrame;

	Player(Texture& image) {
		sprite.setTexture(image);
		rect = FloatRect(1 * ts, 8 * ts, 42, 48);
		sprite.setTextureRect(IntRect(56, 8, 26, 26));
		sprite.setScale(2.0, 2.0);

		dx = dy = 0;
		curFrame = 0;
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
		if (rect.left == 0)
		{
			rect.left = ts;
		}
		Collision(1);
		if (rect.top > ground)
		{
			rect.top = ground;
			dy = 0;
			onGround = 1;
		}
			sprite.move(0.2 * time, 0);
			if(dx>0)
			sprite.setTextureRect(IntRect(56, 8, 26, 26));
			if (dx < 0)  sprite.setTextureRect(IntRect(56 + 26, 8, -26, 26));
			sprite.setPosition(rect.left, rect.top);
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

int main()
{
	RenderWindow window(VideoMode(1000, 500), "Mega Man");
	Texture Picture;
	Picture.loadFromFile("C:/random_game_files/finalsprites.png");
	Player p (Picture);

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
			if (event.type == Event::Closed) window.close();
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
		p.update(time);
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
				platform.setPosition(j * ts, i * ts);
				window.draw(platform);
			}
		}

		window.draw(p.sprite);
		window.display();
	}
	return 0;
	//commment
}