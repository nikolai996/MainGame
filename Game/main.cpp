#include <SFML/Graphics.hpp>
#include <iostream> 
#include "view.h"
#include "level.h"
#include <vector>
class Entity {
public:
	std::vector<Object> obj;
	float dx, dy, x, y, speed, moveTimer;
	int w, h, health;
	bool life, isMove, isSelect;
	Texture texture;
	Sprite sprite;
	String name;
	Entity(Image &image, float X, float Y, int W, int H, String Name) {
		x = X; y = Y; w = W; h = H; name = Name; moveTimer = 0;
		speed = 0; health = 100; dx = 0; dy = 0;
		life = true; isMove = false;
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setOrigin(w / 2, h / 2);
	}
	FloatRect getRect() {
		return FloatRect(x, y, w, h);
	}
	virtual void update(float time) = 0;
};

#include <list>
using namespace sf;

class Player :public Entity {
public:
	enum { left, right, up, down, leftUp, rightUp, leftDown, rightDown, stay } state;
	float rotation;
	int playerScore;
	Player(Image &image, Level &lev, float X, float Y, int W, int H, String Name) :Entity(image, X, Y, W, H, Name) {
		playerScore = 0; state = stay; isSelect = false; obj = lev.GetAllObjects();


		if (name == "Player") {
			sprite.setPosition(w, h);
		}
	}

	void control() {
		bool pressBut = false;
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			state = left; speed = 0.2;
			pressBut = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			state = right; speed = 0.2;
			pressBut = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			if (pressBut) {
				if (state == right) {
					state = rightUp; speed = 0.2;
				}
				if (state == left) {
					state = leftUp; speed = 0.2;
				}
			}
			else {
				state = up; speed = 0.2;
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			if (pressBut) {
				if (state == right) {
					state = rightDown; speed = 0.2;
				}
				if (state == left) {
					state = leftDown; speed = 0.2;
				}
			}
			else {
				state = down; speed = 0.2;
			}
		}
	}
	void rotation_GG(Vector2f pos) {
		float dX = pos.x - x;
		float dY = pos.y - y;
		rotation = (atan2(dY, dX)) * 180 / 3.14159265;
	}
	void checkCollisionWithMap(float Dx, float Dy)
	{
		for (int i = 0; i<obj.size(); i++)
			if (getRect().intersects(obj[i].rect))
			{
				if (obj[i].name == "solid")
				{
					if (Dy>0) { y = obj[i].rect.top - h;  dy = 0; }
					if (Dy<0) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (Dx>0) { x = obj[i].rect.left - w; }
					if (Dx<0) { x = obj[i].rect.left + obj[i].rect.width; }
				}
			}
	}

	void update(float time)
	{
		sprite.setRotation(rotation);
		control();
		switch (state)
		{
		case right:dx = speed; dy = 0;break;
		case rightUp: dx = speed; dy = -speed; break;
		case rightDown: dx = speed; dy = speed; break;
		case left:dx = -speed; dy = 0; break;
		case leftUp: dx = -speed; dy = -speed; break;
		case leftDown: dx = -speed; dy = speed; break;
		case up: dx = 0; dy = -speed; break;
		case down: dx = 0; dy = speed; break;
		case stay: break;
		}
		x += dx*time;
		checkCollisionWithMap(dx, 0);
		y += dy*time;
		checkCollisionWithMap(0, dy);
		sprite.setPosition(x + w / 2, y + h / 2);

		if (health <= 0) { life = false; }
		if (!isMove) { speed = 0; }
		if (life) { getPlayerCoordinateForView(x, y); }
	}
};

class Enemy :public Entity {
public:
	Enemy(Image &image, Level &lvl, float X, float Y, int W, int H, String Name) :Entity(image, X, Y, W, H, Name) {
		obj = lvl.GetObjects("solid");
		if (name == "easyEnemy") {
			//sprite.setTextureRect(IntRect(0, 0, w, h));
			dx = -0.1;

		}
	}

	void checkCollisionWithMap(float Dx, float Dy){
		for (int i = 0; i<obj.size(); i++) {
			if (getRect().intersects(obj[i].rect)) {
				if (obj[i].name == "solid") {
					if (Dy > 0) {
						y = obj[i].rect.top - h;
						dy = -0.1;
					}
					if (Dy < 0) {
						y = obj[i].rect.top + obj[i].rect.height;
						dy = 0.1;
					}
					if (Dx > 0) {
						x = obj[i].rect.left - w;
						dx = -0.1;
					}
					if (Dx < 0) {
						x = obj[i].rect.left + obj[i].rect.width;
						dx = 0.1;
					}
				}
			}
		}
	}

	void update(float time){
		if (name == "easyEnemy") {
			checkCollisionWithMap(dx, dy);
			x += dx*time;
			y += dy*time;
			sprite.setPosition(x + w / 2, y + h / 2);
			if (health <= 0) { life = false; }
		}
	}
};

class Bullet :public Entity {
public:
	int direction;
	float tempy, tempx, rotation, Dx, Dy;
	Bullet(Image &image, Level &lvl, float X, float Y, int W, int H, float tempX, float tempY, String Name) :Entity(image, X, Y, W, H, Name) {
		obj = lvl.GetObjects("solid");
		x = X;
		y = Y;
		speed = 0.1;
		tempx = tempX;
		tempy = tempY;
		w = W;
		h = H;
		life = true;
		dx = x;
		dy = y;
		life = true;
		Dx = tempx - x;
		Dy = tempy - y;
		rotation = (atan2(Dy, Dx)) * 180 / 3.14159265;
	}

	void update(float time)
	{
		x += speed * (tempx - dx);
		y += speed * (tempy - dy);

		if (x <= 0) x = 1;
		if (y <= 0) y = 1;

		for (int i = 0; i < obj.size(); i++) {
			if (getRect().intersects(obj[i].rect))
			{
				life = false;
			}
		}
		sprite.setRotation(rotation);
		sprite.setPosition(x + w / 2, y + h / 2);
	}
};

int main(){
	int tempX = 0;
	int tempY = 0;
	float distance = 0;
	sf::RenderWindow window(sf::VideoMode(1370, 768), "Game");
	view.reset(sf::FloatRect(0, 0, 1370, 768));

	Level lvl;
	lvl.LoadFromFile("/home/nikolai/ClionProjects/Game/map.tmx");

	Object player = lvl.GetObject("player");

	std::list<Entity*>  entities;
	std::list<Entity*>::iterator it;

	std::vector<Object> e = lvl.GetObjects("easyEnemy");


	Image heroImage, easyEnemyImage, bulletImage;
	bulletImage.loadFromFile("/home/nikolai/ClionProjects/Game/IMG/projectile_bolt_blue_single.png");
	heroImage.loadFromFile("/home/nikolai/ClionProjects/Game/IMG/PlayerShip.png");
	easyEnemyImage.loadFromFile("/home/nikolai/ClionProjects/Game/IMG/EasyEnemy.png");
	std::cout << player.rect.left << player.rect.top;
	Player p(heroImage, lvl, player.rect.left, player.rect.top, 91, 54, 72, 20, "Player");
	Clock clock;

	for (int i = 0; i < e.size(); i++)
		entities.push_back(new Enemy(easyEnemyImage, lvl, e[i].rect.left, e[i].rect.top, 43, 75, 0, 0, "easyEnemy"));

	bool ret = false;

	float CurrentFrame = 0;
	while (window.isOpen())
	{
		std::list<Entity*>::iterator at;
		Vector2i pixelPos = Mouse::getPosition(window);
		Vector2f pos = window.mapPixelToCoords(pixelPos);
		float time = clock.getElapsedTime().asMicroseconds();

		clock.restart();
		time = time / 800;
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.key.code == Mouse::Left) {
				entities.push_back(new Bullet(bulletImage, lvl, p.x, p.y, 23, 7, pos.x, pos.y, "Bullet"));
			}
		}
		p.rotation_GG(pos);
		p.update(time);// Player update function
		for (it = entities.begin(); it != entities.end();)
		{
			Entity *b = *it;
			b->update(time);
			if (b->life == false) { it = entities.erase(it); delete b; }
			else it++;
		}
		for (it = entities.begin(); it != entities.end(); it++)
		{
			for (at = entities.begin(); at != entities.end(); at++) {
				if ((*it)->getRect().intersects((*at)->getRect()) && (((*at)->name == "Bullet") && ((*it)->name == "easyEnemy"))) {
					(*it)->health -= 13;
					(*at)->life = false;
				}
			}
			if ((*it)->getRect().intersects(p.getRect()))
			{
				if ((*it)->name == "easyEnemy") {
					(*it)->dx = 0;
					p.health -= 1;
				}
			}
		}
		if (!p.life)
			window.close();
		//for (it = entities.begin(); it != entities.end(); it++) { (*it)->update(time); }
		window.setView(view);
		window.clear();
		lvl.Draw(window);
		for (it = entities.begin(); it != entities.end(); it++) {
			window.draw((*it)->sprite);
		}
		//window.draw(easyEnemy.sprite);
		window.draw(p.sprite);
		window.display();
	}

	return 0;
}
