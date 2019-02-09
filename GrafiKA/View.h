#include <SFML/Graphics.hpp>
using namespace sf;
View view;
View getPlayerCoordinateForView(float x, float y)
{
	view.setCenter(x + 50, y);
	return view;
}

View viewmap(float time)
{
	if (Keyboard::isKeyPressed(Keyboard::D))
	{
		view.move(0.1f*time, 0);
	}
	if (Keyboard::isKeyPressed(Keyboard::S))
	{
		view.move(0, 0.1f * time);
	}
	if (Keyboard::isKeyPressed(Keyboard::A))
	{
		view.move(-0.1f*time, 0);
	}
	if (Keyboard::isKeyPressed(Keyboard::W))
	{
		view.move(0, -0.1f * time);
	}
	return view;
}