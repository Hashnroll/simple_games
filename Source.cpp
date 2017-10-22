#include <SFML/Graphics.hpp>
using namespace sf;

const int W = 800;
const int H = 600;

int ballRadius = 10;
int playerWidth = 100;
int playerHeight = 50;

int main()
{
	srand(time(0));

	VideoMode vm;
	RenderWindow app(VideoMode(W, H), "Arcanoid");
	app.setFramerateLimit(60);

	RectangleShape player(Vector2f(playerWidth, playerHeight));
	player.setOrigin(playerWidth*0.5, 0);
	player.setFillColor(Color::Yellow);
	player.setPosition(W*0.5, H - 5 - playerHeight);
	CircleShape ball(ballRadius);
	ball.setOrigin(ballRadius, ballRadius);
	ball.setPosition(W * 0.5, H * 0.5);

	const int velocity = 5;
	float dx = (static_cast<float>(rand()) / RAND_MAX) * velocity;
	if (rand() % 2 == 0)
		dx = -dx;
	float dy = sqrt(velocity*velocity-dx*dx);

	while (app.isOpen())
	{
		Event event;
		while (app.pollEvent(event))
		{
			if (event.type == Event::Closed)
				app.close();
		}

		app.clear(Color(110, 100, 130));

		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			if (player.getPosition().x + playerWidth*0.5 < W)
				player.move(5, 0);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			if (player.getPosition().x - playerWidth*0.5 > 0)
				player.move(-5, 0);
		}

		if (((ball.getPosition().y + ballRadius >= player.getPosition().y) && (ball.getPosition().x >= player.getPosition().x - playerWidth * 0.5 && ball.getPosition().x <= player.getPosition().x + playerWidth * 0.5)))
		{
			dy = -dy;
		}
		if (ball.getPosition().x <= 0 || ball.getPosition().x >= W)
			dx = -dx;

		if (ball.getPosition().y <= 0)
			dy = -dy;

		ball.move(dx, dy);

		app.draw(player);
		app.draw(ball);

		app.display();

		if (ball.getPosition().y > H)
			app.close();

	}

	return 0;
}