#include <SFML/Graphics.hpp>
#include <list>
#include <time.h>
#include <iostream>
using namespace sf;

const int N = 50;
const int M = 20;
const int cellSize = 30;
const int W = N*cellSize;
const int H = M*cellSize;;

bool keyBlock = false;
int keyBuffer = -1;

class Snake
{
public:
	int length = 2;
	RectangleShape* body[100];
	int direction; //0-right, 1-up, 2-left, 3-down
	int speed;
	int count = 0;

	Snake()
	{
		RectangleShape* head = new RectangleShape(Vector2f(cellSize - 1, cellSize - 1));
		int i = rand() % M;
		int j = rand() % N;
		head->setPosition(j*cellSize, i*cellSize);
		head->setFillColor(Color(200,200,200));
		body[0] = head;
		RectangleShape* tail = new RectangleShape(Vector2f(cellSize - 1, cellSize - 1));
		tail->setPosition((j - 1)*cellSize, cellSize*i);
		tail->setFillColor(Color::Black);
		body[1] = tail;
		direction = 0;
		speed = cellSize;
	}

	bool checkCollide()
	{
		Vector2f headPos = body[0]->getPosition();
		for (int i = 1; i < length; i++)
		{
			Vector2f pos = body[i]->getPosition();
			if (headPos.x == pos.x && headPos.y == pos.y)
				return true;
		}
		return false;
	}

	void setDirection(int x)
	{
		direction = x;
	}

	void update(RenderWindow& app)
	{
		keyBlock = false;
		count = 0;

		if (keyBuffer >= 0) {
			direction = keyBuffer;
			keyBuffer = -1;
		}

		for (int i = length - 1; i > 0; i--)
		{
			Vector2f prevPos = body[i - 1]->getPosition();
			body[i]->setPosition(prevPos.x, prevPos.y);
		}

		RectangleShape* c = body[0];

		switch (direction)
		{
		case 0:
		{
			c->move(speed, 0);
			break;
		}
		case 1:
		{
			c->move(0, speed);
			break;
		}
		case 2:
		{
			c->move(-speed, 0);
			break;
		}
		case 3:
		{
			c->move(0, -speed);
			break;
		}
		default:
			break;
		}

		Vector2f pos = c->getPosition();
		if (pos.x < 0)
			//app.close();
			c->setPosition(W, pos.y);
		if (pos.x == W)
			//app.close();
			c->setPosition(0, pos.y);
		if (pos.y < 0)
			//app.close();
			c->setPosition(pos.x, H);
		if (pos.y == H)
			//app.close();
			c->setPosition(pos.x, 0);
	}

	void grow()
	{
		RectangleShape* newRect = new RectangleShape(Vector2f(cellSize - 1, cellSize - 1));
		newRect->setFillColor(Color::Black);
		newRect->setPosition(-cellSize, -cellSize); //invincible until next update
		length++;
		body[length - 1] = newRect;
	}

	void display(RenderWindow &app)
	{
		for (int i = 0; i < length; ++i) {
			app.draw(*body[i]);
		}
	}

};

int appleRadius = 10;
CircleShape apple(appleRadius);

int main()
{
	srand(time(0));

	VideoMode vm;
	RenderWindow app(VideoMode(W, H), "Acid Snake");
	app.setFramerateLimit(120);

	Snake* snake = new Snake();

	apple.setOrigin(appleRadius, appleRadius);
	apple.setFillColor(Color::Green);
	int i = rand() % M;
	int j = rand() % N;
	apple.setPosition(cellSize*j + 0.5*cellSize, cellSize*i + 0.5*cellSize);

	bool appleIsEaten = false;

	Clock clock;
	float timer = 0.0, delay = 0.1;

	while (app.isOpen())
	{
		Event event;
		while (app.pollEvent(event))
		{
			if (event.type == Event::Closed)
				app.close();
		}

		app.clear(Color(110, 100, 130));

		for (int i = cellSize; i < H; i += cellSize) {
			sf::Vertex line[] = { sf::Vertex(sf::Vector2f(0, i)), sf::Vertex(sf::Vector2f(W, i)) };
			app.draw(line, 2, sf::Lines);
			for (int j = cellSize; j < W; j += cellSize)
			{
				sf::Vertex line[] = { sf::Vertex(sf::Vector2f(j, 0)), sf::Vertex(sf::Vector2f(j, H)) };
				app.draw(line, 2, sf::Lines);
			}
		}

		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			if (snake->direction != 2 && !keyBlock)
			{
				snake->setDirection(0);
				keyBlock = true;
			}
			else if (keyBlock && keyBuffer == -1)
			{
				keyBuffer = 0;
			}
		}
		else if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			if (snake->direction != 3 && !keyBlock)
			{
				snake->setDirection(1);
				keyBlock = true;
			}
			else if (keyBlock && keyBuffer == -1)
			{
				keyBuffer = 1;
			}
		}
		else if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			if (snake->direction != 0 && !keyBlock)
			{
				snake->setDirection(2);
				keyBlock = true;
			}
			else if (keyBlock && keyBuffer == -1)
			{
				keyBuffer = 2;
			}
		}
		else if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			if (snake->direction != 1 && !keyBlock)
			{
				snake->setDirection(3);
				keyBlock = true;
			}
			else if (keyBlock && keyBuffer == -1)
			{
				keyBuffer = 3;
			}
		}

		RectangleShape* head = (snake->body)[0];
		float headX = head->getPosition().x + cellSize*0.5;
		float headY = head->getPosition().y + cellSize*0.5;
		if (headX == apple.getPosition().x && headY == apple.getPosition().y)
			appleIsEaten = true;

		if (appleIsEaten)
		{
			snake->grow();
			i = rand() % M;
			j = rand() % N;
			int k = 0;
			while (k < snake->length)
			{
				Vector2f pos = snake->body[k]->getPosition();
				if (j * cellSize == pos.x && i * cellSize == pos.y)
				{
					i++; i %= M;;
					j++; j %= N;
					k = 0;
					continue;
				}
				k++;
			}

			apple.setPosition(cellSize*(j + 0.5), cellSize*(i + 0.5));
			appleIsEaten = false;
		}

		if (timer > delay)
		{
			snake->update(app);
			timer = 0;
		}
		snake->display(app);

		app.draw(apple);

		app.display();

		if (snake->checkCollide())
			app.close();
	}
	return 0;
}