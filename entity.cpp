#pragma once
#include "entity.h"

// Делаем приватный спрайт доступным для функции draw()
Sprite Entity::getSprite()
{
    return m_Sprite;
}
void Entity::setPosition(Vector2f pos) {
    position = pos;
}
Vector2f Entity::getPosition() {
    return position;
}