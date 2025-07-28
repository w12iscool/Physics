//
// Created by Harshad on 7/17/2025.
//
#pragma once

#include "raylib.h"
#include "box2d/box2d.h"

#include "Box.h"
#include "Ball.h"
#include "Bow.h"
#include "Dagger.h"
#include "Shield.h"
#include "spear.h"
#include "Sword.h"

class Box;
class Ball;
class Item;
class Spear;
class Sword;
class Bow;
class GameEngine {
private:
    b2WorldId m_worldId = {0};
private:
    bool m_gameFrozen = false;
    Box box;
    Ball ball;
    Ball ball2;
    Item testItem;
    Spear spear;
    Sword sword;
    Dagger dagger;
    Shield shield;
    Bow bowAndArrow;
    std::vector<Weapon*> m_weapons;
public:
    void startUp();
    void update();
    void render();
    void shutDown();
};




