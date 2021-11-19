#pragma once
#include <SFML/Graphics.hpp>
#include "entity.h"

using namespace sf;

class Bullet : public Entity {
    double speed;
public:
    double getSpeed();
    void update(float x, float y);
    Bullet();
};

class Gun : public Entity
{
private:
    // Логические переменные для отслеживания направления движения
    bool m_UpPressed;
    bool m_DownPressed;
    bool fire;
    // Скорость Боба в пикселях в секунду
    Bullet* bullet;
    float angle;
    float start_x;
    float start_y;
    float fire_x;
    float fire_y;
    float fire_angle;
    float fire_fly_time;

    void updateTexture();
public:
    // Настраиваем Боба в конструкторе
    Gun();
    // Для отправки спрайта в главную функцию
    Bullet& const getBullet();
    Vector2f getStartPosition();
    Vector2f getFirePosition();
    Vector2f correctStartPosition();
    float getFireAngle();
    void setFireAngle();
    float getFireTime();
    void setFireTime(float time);
    void setFirePosition();

    void update(float elapsedTime);
    float getAngle();
    bool isOnFire();
    // Для движения Боба
    void moveUp();
    void moveDown();
    void shot();
    // Прекращение движения
    void stopUp();
    void stopDown();
    void stopShot();
    ~Gun()
    {
        delete bullet;
    }
};