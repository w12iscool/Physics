//
// Created by Harshad on 7/17/2025.
//
#pragma once

#include "raylib.h"
#include "box2d/box2d.h"

#include "Box.h"
#include "Ball.h"
#include "spear.h"

class Box;
class Ball;
class Item;
class Spear;

class GameEngine {
private:
    b2WorldId m_worldId = {0};
private:
    Box box;
    Ball ball;
    Ball ball2;
    Item testItem;
    Spear spear;
public:
    void startUp();
    void update();
    void render();
    void shutDown();
};




