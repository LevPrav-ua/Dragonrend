#include "gun.h"
#include <iostream>

Gun::Gun()
{
    // Вписываем в переменную скорость Боба
    angle =  45.0;
    fire = false;

    // Связываем текстуру и спрайт
    m_Texture.loadFromFile("source\\gun\\gun_150.png");
    m_Sprite.setTexture(m_Texture);
    
    // Устанавливаем начальную позицию Боба в пикселях
    position.x = 10;
    position.y = 590;
    start_x = getPosition().x + getSprite().getTexture()->getSize().x;
    start_y = getPosition().y - getAngle() / 2;
    bullet = new Bullet();
    bullet->setPosition({start_x, start_y});
}

void Gun::moveUp()
{
    m_UpPressed = true;
}
void Gun::moveDown()
{
    m_DownPressed = true;
}
void Gun::stopUp()
{
    m_UpPressed = false;
}
void Gun::stopDown()
{
    m_DownPressed = false;
}
void Gun::shot() {
    fire = true;
}
void Gun::stopShot() {
    fire = false;
}
bool Gun::isOnFire() {
    return fire;
}
Vector2f Gun::getStartPosition() {
    return { start_x, start_y };
}
Vector2f Gun::correctStartPosition() {
    float x = 0.0, y = 0.0;
    if (angle < 16.0) {
        y = 35.0;
    }
    else if (angle < 26.0) {
        y = 20.0;
    }
    else if (angle < 36.0) {
        x = 10.0;
        y = 5.0;
    }
    else if (angle < 46.0) {
        x = 10.0;
        y = -5.0;
    }
    else if (angle < 56.0) {
        x = -20.0;
        y = -20.0;
    }
    else if (angle < 66.0) {
        x = -35.0;
        y = -30.0;
    }

    return { x,y };
}

void Gun::updateTexture() {
    if (angle == 10.0) {
        m_Texture.loadFromFile("source\\gun\\gun_150.png");
        m_Sprite.setTexture(m_Texture);
    }
    if (angle == 15.0) {
        m_Texture.loadFromFile("source\\gun\\gun_sprites\\gun_15.png");
        m_Sprite.setTexture(m_Texture);
    }
    if (angle == 25.0) {
        m_Texture.loadFromFile("source\\gun\\gun_sprites\\gun_25.png");
        m_Sprite.setTexture(m_Texture);
    }
    if (angle == 35.0) {
        m_Texture.loadFromFile("source\\gun\\gun_sprites\\gun_40.png");
        m_Sprite.setTexture(m_Texture);
    }
    if (angle == 45.0) {
        m_Texture.loadFromFile("source\\gun\\gun_sprites\\gun_50.png");
        m_Sprite.setTexture(m_Texture);
    }
    if (angle == 55.0) {
        m_Texture.loadFromFile("source\\gun\\gun_sprites\\gun_60.png");
        m_Sprite.setTexture(m_Texture);
    }
}

void Gun::update(float elapsedTime)
{
    if (m_DownPressed)
    {
        if (angle > 10 ) angle -= 0.5;
    }
    if (m_UpPressed)
    {
        if (angle < 65) angle += 0.5;
    }
    updateTexture();
    // Теперь сдвигаем спрайт на новую позицию
    m_Sprite.setPosition(position);
}

Bullet& const Gun::getBullet(){
    return *bullet;
}
float Gun::getAngle() {
    return angle;
}
float Gun::getFireAngle() {
    return fire_angle;
}
float Gun::getFireTime() {
    return fire_fly_time;
}
Vector2f Gun::getFirePosition() {
    return { fire_x, fire_y };
}

void Gun::setFireAngle() {
    fire_angle = angle;
}
void Gun::setFireTime(float time) {
    fire_fly_time = time;
}
void Gun::setFirePosition() {
    auto correct = correctStartPosition();
    fire_x = getPosition().x + getSprite().getTexture()->getSize().x + correct.x;
    fire_y = getPosition().y - getAngle() / 2 + 10 + correct.y;
}

Bullet::Bullet() {
    // Связываем текстуру и спрайт
    speed = 100;

    if (!m_Texture.loadFromFile("source\\gun\\bullet.png")) {
        std::cerr << "Bullet pic wasn't load\n";
    }
    m_Sprite.setTexture(m_Texture);
    m_Sprite.setPosition(0,0);
}

void Bullet::update(float x, float y) {
    position = { x,y };
    m_Sprite.setPosition({x, y});
}

double Bullet::getSpeed() {
    return speed;
}
