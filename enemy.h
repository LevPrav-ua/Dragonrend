#pragma once
#include "entity.h"
#include "gun.h"

class Enemy : public Entity {
	float speed;
	bool alive;
	float timer;
	int frame;
	float accuracy;
public:
	void kill();
	void born(int max_x, int max_y);
	bool hitCheck(Bullet& bullet);
	bool outOfWindow();
	bool isAlive();
	void changeFrame();
	Enemy();

	void update(float elapsedTime);
};