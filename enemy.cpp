#include <iostream>
#include "enemy.h"

Enemy::Enemy() {
    // Связываем текстуру и спрайт
    if (!m_Texture.loadFromFile("source\\enemy\\fly_up.png")) {
        std::cerr << "Enemy pic wasn't load\n";
    }
    m_Sprite.setTexture(m_Texture);

    speed = 70.0;
    alive = false;
    accuracy = 0.75;
    timer = 0.0;
    frame = 0;
}

void Enemy::update(float elapsedTime) {
    if (alive) {
        timer += elapsedTime;
        if (timer >= 0.1) {
            frame = (frame < 3) ? frame + 1 : 0;
            timer = 0.0;
            changeFrame();
        }
        position.x -= elapsedTime * speed;
        m_Sprite.setPosition(position.x, position.y);
    }
}

void Enemy::changeFrame() {
    switch (frame)
    {
        case 0: 
            m_Texture.loadFromFile("source\\enemy\\fly_up.png");
            break;
        case 1:
            m_Texture.loadFromFile("source\\enemy\\fly_mid.png");
            break;
        case 2:
            m_Texture.loadFromFile("source\\enemy\\fly_down.png");
            break;
        case 3:
            m_Texture.loadFromFile("source\\enemy\\fly_mid.png");
            break;
    default:
        break;
    }
    m_Sprite.setTexture(m_Texture);
}

bool Enemy::outOfWindow() {
    if (position.x + float(m_Texture.getSize().x) <= 0.0) {
        return true;
    }
    return false;
}
bool Enemy::isAlive() {
    return alive;
}
bool Enemy::hitCheck(Bullet& bullet) {
    Vector2f bullet_pos = bullet.getPosition();
    float x_center = position.x + (m_Texture.getSize().x/2) , 
        y_center = position.y + (m_Texture.getSize().y/2);
    float radius = (m_Texture.getSize().x / 2) * accuracy;

    if ((bullet_pos.x > x_center - radius) && (bullet_pos.x < x_center + radius)) {
        if ((bullet_pos.y > y_center - radius) && (bullet_pos.y < y_center + radius)) {
            return true;
        }
    }
    return false;
}
void Enemy::kill() {
    alive = false;
}
void Enemy::born(int max_x, int max_y) {
    setPosition({ float(max_x), float(rand() % (max_y / 4 ))});
    alive = true;
}