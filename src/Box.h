#pragma once

#include "raylib.h"
#include "box2d/box2d.h"
#include "Constants.h"

class Box {
private:
    int m_size = { 500 };
    Vector2 m_pos = Vector2((SCREEN_WIDTH - m_size) / 2, (SCREEN_HEIGHT - m_size) / 2);
    Rectangle m_boxRect = Rectangle(m_pos.x, m_pos.y, m_size, m_size);
private:
    b2BodyId m_leftWallId = { 0 };
    b2BodyId m_rightWallId = { 0 };
    b2BodyId m_ceilingId = { 0 };
    b2BodyId m_floorId = { 0 };
public:
    int getSize() const;
    Vector2 getPos();
    Rectangle getRect();

    void renderRect();
    void initBox(b2WorldId& worldId);
    void renderBox2dBoxHitBox();
};


