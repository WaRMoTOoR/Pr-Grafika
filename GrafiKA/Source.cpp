#include "SFML/Graphics.hpp"
#include <iostream>
#include <sstream>
#include "Map.h"
#include "View.h"
#include "Mission.h"
#include <vector>
#include <list>


using namespace sf;
using namespace std;

class Entity {
public:
	float dx, dy, x, y, speed, moveTimer;
	int w, h, health;
	bool life, isMove, onGround;
	Texture texture;
	Sprite sprite;
	String name;
	Entity(Image &image, float X, float Y, int W, int H, String Name) {
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true; onGround = false; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}
};

class Player:public Entity
{
public: 
	int MainHeroScore;
	enum {left, right, up, down, jump, stay} state;

	Player(Image &image, float X, float Y, int W, int H, String Name):Entity(image, X, Y, W, H, Name)
	{
		MainHeroScore = 0;
		state = stay;
		if (name == "Player1") 
			{
				sprite.setTextureRect(IntRect(0, 0, w, h));
			}
	}

	void update(float time)
	{
		control();
		switch (state)
		{
		case right: dx = speed; break;
		case left: dx = -speed;  break;
		case up:  break;
		case down: dx = 0; break;
		case stay: break;
		}
		   x += dx*time;
		   checkCollisionWithMap(dx, 0);
		   y += dy*time;
		   checkCollisionWithMap(0, dy);
		   sprite.setPosition(x+w/2,y+h/2);
		   if (health <= 0){ life = false; }
		   if (!isMove){ speed = 0; }
		   getPlayerCoordinateForView(x, y);
		   if (life) { getPlayerCoordinateForView(x, y); }
		   dy = dy + 0.0015*time;
	}

	void checkCollisionWithMap(float Dx, float Dy)
	{
		for (int i = y / 32; i < (y + h) / 32; i++)
			for (int j = x / 32; j < (x + w) / 32; j++)
			{
				if (TileMap[i][j] == '0')
				{
					if (Dy > 0) { y = i * 32 - h;  dy = 0; onGround = true; }
					if (Dy < 0) { y = i * 32 + 32;  dy = 0; }
					if (Dx > 0) { x = j * 32 - w; }
					if (Dx < 0) { x = j * 32 + 32; }
				}
				if (TileMap[i][j] == 's')
				{
					x = 100, y = 200;
					health -= 40;
				}
				if (TileMap[i][j] == 'f')
				{
					MainHeroScore = MainHeroScore++;
					health += 20;
					TileMap[i][j] == ' ';
				}
			}
	}
	void control()
	{
		if (Keyboard::isKeyPressed) {
			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				state = left; speed = 0.1;
			}
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				state = right; speed = 0.1;
			}

			if ((Keyboard::isKeyPressed(Keyboard::Up)) && (onGround)) {
				state = jump; dy = -0.6; onGround = false;
			}

			if (Keyboard::isKeyPressed(Keyboard::Down)) {
				state = down;
			}
		}
	}
};

class Enemy :public Entity {
public:
	Enemy(Image &image, float X, float Y, int W, int H, String Name) :Entity(image, X, Y, W, H, Name) {
		if (name == "EasyEnemy") {
			sprite.setTextureRect(IntRect(50, 65, w, h));
			dx = 0.1;
		}
	}

	void checkCollisionWithMap(float Dx, float Dy)
	{
		for (int i = y / 32; i < (y + h) / 32; i++)
			for (int j = x / 32; j < (x + w) / 32; j++)
			{
				if (TileMap[i][j] == '0')
				{
					if (Dy > 0) { y = i * 32 - h; }
					if (Dy < 0) { y = i * 32 + 32; }
					if (Dx > 0) { x = j * 32 - w; dx = -0.1; sprite.scale(-1, 1); }
					if (Dx < 0) { x = j * 32 + 32; dx = 0.1; sprite.scale(-1, 1); }
				}

			}
	}

	void update(float time)
	{
		if (name == "EasyEnemy") {

			moveTimer += time;
			if (moveTimer>3000)
			{ 
				dx *= -1; 
				moveTimer = 0; 
			}
			checkCollisionWithMap(dx, 0);
			x += dx * time;
			sprite.setPosition(x + w / 2, y + h / 2);
			if (health <= 0) { life = false; }
		}
	}
};


int main()
{
	RenderWindow window(VideoMode(1024, 768), "RPG");
	view.reset(FloatRect(21, 11, 640, 480));

	Image map_image;
	map_image.loadFromFile("images/Overworld.png");
	Texture map_texture;
	map_texture.loadFromImage(map_image);
	Sprite map_sprite;
	map_sprite.setTexture(map_texture);

	Clock clock;

	Image heroImage;
	heroImage.loadFromFile("images/character.png");

	Image easyEnemyImage;
	easyEnemyImage.loadFromFile("images/characters.png");

	Player p(heroImage, 100, 200, 17, 30, "Player1");
	Enemy easyEnemy(easyEnemyImage, 850, 630, 15, 15, "EasyEnemy");

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		
		clock.restart();
		time = time / 800;

		Event event;

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		easyEnemy.update(time);
		window.setView(view);
		window.clear();

		for (int i = 0; i < height_map; i++)
		for (int j = 0; j < width_map; j++)
			{
			if (TileMap[i][j] == ' ') map_sprite.setTextureRect(IntRect(70, 146, 32, 32));
			if (TileMap[i][j] == 's') map_sprite.setTextureRect(IntRect(39, 103, 32, 32));
			if (TileMap[i][j] == '0') map_sprite.setTextureRect(IntRect(208, 240, 32, 32));
			if (TileMap[i][j] == 'f') map_sprite.setTextureRect(IntRect(520, 350, 32, 32));
			if (TileMap[i][j] == 'h') map_sprite.setTextureRect(IntRect(208, 240, 32, 32));
			map_sprite.setPosition(j * 32, i * 32);
			
			window.draw(map_sprite);
			}
		p.update(time);
		window.draw(p.sprite);
		window.draw(easyEnemy.sprite);
		window.display();
	}

	return 0;
}