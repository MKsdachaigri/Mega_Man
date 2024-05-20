#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

int ground = 250;

class Player {
public:
	float dx, dy;
	FloatRect rect;
	bool onGround;
	Sprite sprite;
	float curFrame;

	Player(Texture& image) {
		sprite.setTexture(image);
		sprite.setTextureRect(IntRect(56, 8, 26, 26));

		dx = dy = 0;
		curFrame = 0;
	}

	void update(float time) {
		rect.left += dx * time;
		if (!onGround)
		{
			dy = dy + 0.0005 * time;
		}
		rect.top += dy * time;
		onGround = 0;
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

};

int main()
{
	RenderWindow window(VideoMode(1000, 500), "Mega Man walks");
	Texture Picture;
	Picture.loadFromFile("C:/random_game_files/finalsprites.png");
	Player p (Picture);
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
		window.draw(p.sprite);
		window.display();
	}
	return 0;
	//commment
}