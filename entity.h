#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Entity {
protected:
    Vector2f position;
    // Объявляем объект Sprite
    Sprite m_Sprite;
    // Добавляем текстуру
    Texture m_Texture;
public:
    Sprite getSprite();
    void setPosition(Vector2f pos);
    Vector2f getPosition();
    void update(float elapsedTime) {};
};