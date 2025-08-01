//
// Created by Harshad on 7/17/2025.
//
#pragma once

#include "raylib.h"
#include "box2d/box2d.h"
#include "Constants.h"
#include "Item.h"
#include "Random.h"
#include "RaylibTimer.h"
#include <string>



class Item;
namespace hitItem
{
    enum itemHit
    {
        None,
        Spear,
        Sword
    };
}

class Ball {
private:
    float m_radius{ 30 };
    Vector2 m_pos = Vector2((SCREEN_WIDTH - m_radius) / 2, (SCREEN_HEIGHT - m_radius) / 2);
    Color m_defaultColor{ RED };
    Color m_color{ RED };
    bool m_isFrozen{ false };
    bool m_freezeDebounce{ false };
    int m_health{ 100 };
    hitItem::itemHit m_whatHitMe = hitItem::itemHit::None;

private:
    b2BodyId m_ballId = { 0 };
    b2Vec2 previousVelocity;
public:
    int getRadius() const;
    Vector2 getPos();

    void initBallBox2d(b2WorldId& worldId);
    void renderCircle();
    b2BodyId getBallId();
    void testStopTime();
    void setPos(Vector2 newPos);
    void keepMoving(bool& gameFrozen);
    void setFrozen(bool frozen);
    void setColor(Color newColor);
    void takeDamage(int damage);
    bool getFrozen();
    hitItem::itemHit getItemHit();
    void setWhoHitMe(hitItem::itemHit item);

    void handleFreezing(float& orbitSpeed, float normalOrbitSpeed, Ball& ball, bool& gameFrozen);
    void handleColor(hitItem::itemHit hostItem);

    void handleDeath();

    int getHealth();
    void initDefaultColor(Color color);
    Color getDefaultColor();
    void initFont();
    void setHealth(int health);

    void setDefaultColorR(unsigned char r);
    void setDefaultColorG(unsigned char g);
    void setDefaultColorB(unsigned char b);

    std::unique_ptr<Font> m_font = std::make_unique<Font>();
};




