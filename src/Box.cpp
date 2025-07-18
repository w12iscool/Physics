//
// Created by Harshad on 7/17/2025.
//

#include "Box.h"

int Box::getSize() const
{
    return m_size;
}

Vector2 Box::getPos()
{
    return m_pos;
}

Rectangle Box::getRect()
{
    return m_boxRect;
}

void Box::renderRect()
{
    DrawRectangleLinesEx(m_boxRect, 10, BLACK);
}

void Box::initBox(b2WorldId& worldId)
{
    // Top center
    Vector2 topCenter = {
        m_boxRect.x + m_boxRect.width / 2.0f,
        m_boxRect.y
    };

    // Bottom center
    Vector2 bottomCenter = {
        m_boxRect.x + m_boxRect.width / 2.0f,
        m_boxRect.y + m_boxRect.height
    };

    // Left center
    Vector2 leftCenter = {
        m_boxRect.x,
        m_boxRect.y + m_boxRect.height / 2.0f
    };

    // Right center
    Vector2 rightCenter = {
        m_boxRect.x + m_boxRect.width,
        m_boxRect.y + m_boxRect.height / 2.0f
    };

    b2BodyDef leftWallDef = b2DefaultBodyDef();
    b2BodyDef rightWallDef = b2DefaultBodyDef();
    b2BodyDef ceilingDef = b2DefaultBodyDef();
    b2BodyDef floorDef = b2DefaultBodyDef();
    float pixelsToMeters{ 30.0f };
    float wallWidthOrHeight{ 0.5 };

    leftWallDef.type = b2_staticBody;
    rightWallDef.type = b2_staticBody;
    ceilingDef.type = b2_staticBody;
    floorDef.type = b2_staticBody;

    // Init floor
    floorDef.position = b2Vec2(bottomCenter.x / pixelsToMeters, bottomCenter.y / pixelsToMeters);
    m_floorId = b2CreateBody(worldId, &floorDef);
    b2Polygon groundBox = b2MakeBox((m_size / 30) / 2, wallWidthOrHeight);
    b2ShapeDef groundShapeDef = b2DefaultShapeDef();
    b2CreatePolygonShape(m_floorId, &groundShapeDef, &groundBox);

    // Init left wall
    leftWallDef.position = b2Vec2(leftCenter.x / pixelsToMeters, leftCenter.y / pixelsToMeters);
    m_leftWallId = b2CreateBody(worldId, &leftWallDef);
    b2Polygon leftWallBox = b2MakeBox(wallWidthOrHeight, (m_size / 30) / 2);
    b2ShapeDef leftWallShapeDef = b2DefaultShapeDef();
    b2CreatePolygonShape(m_leftWallId, &leftWallShapeDef, &leftWallBox);

    // Init right wall
    rightWallDef.position = b2Vec2(rightCenter.x / pixelsToMeters, rightCenter.y / pixelsToMeters);
    m_rightWallId = b2CreateBody(worldId, &rightWallDef);
    b2Polygon rightWallBox = b2MakeBox(wallWidthOrHeight, (m_size / 30) / 2);
    b2ShapeDef rightWallShapeDef = b2DefaultShapeDef();
    b2CreatePolygonShape(m_rightWallId, &rightWallShapeDef, &rightWallBox);

    // Init ceiling
    ceilingDef.position = b2Vec2(topCenter.x / pixelsToMeters, topCenter.y / pixelsToMeters);
    m_ceilingId = b2CreateBody(worldId, &ceilingDef);
    b2Polygon ceilingBox = b2MakeBox((m_size / 30) / 2, wallWidthOrHeight);
    b2ShapeDef ceilingShapeDef = b2DefaultShapeDef();
    b2CreatePolygonShape(m_ceilingId, &ceilingShapeDef, &ceilingBox);
}

void Box::renderBox2dBoxHitBox()
{
    float scale = 30.0f;
    float wallThickness = 60.0f; // pixels

    Vector2 ceilingPos = Vector2(b2Body_GetPosition(m_ceilingId).x * scale, b2Body_GetPosition(m_ceilingId).y * scale);
    Vector2 floorPos   = Vector2(b2Body_GetPosition(m_floorId).x   * scale, b2Body_GetPosition(m_floorId).y   * scale);
    Vector2 leftWallPos  = Vector2(b2Body_GetPosition(m_leftWallId).x  * scale, b2Body_GetPosition(m_leftWallId).y  * scale);
    Vector2 rightWallPos = Vector2(b2Body_GetPosition(m_rightWallId).x * scale, b2Body_GetPosition(m_rightWallId).y * scale);

    // Draw floor and ceiling (horizontal)
    DrawRectangle(floorPos.x - m_size / 2, floorPos.y - wallThickness / 2, m_size, wallThickness, BLACK);
    DrawRectangle(ceilingPos.x - m_size / 2, ceilingPos.y - wallThickness / 2, m_size, wallThickness, BLACK);

    // Draw left and right walls (vertical)
    DrawRectangle(leftWallPos.x - wallThickness / 2, leftWallPos.y - m_size / 2, wallThickness, m_size, BLACK);
    DrawRectangle(rightWallPos.x - wallThickness / 2, rightWallPos.y - m_size / 2, wallThickness, m_size, BLACK);
}

