#pragma once
#include "entity.h"

// ������ ��������� ������ ��������� ��� ������� draw()
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