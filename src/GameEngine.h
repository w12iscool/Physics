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
#include "Scythe.h"
#include "Mace.h"
#include "Staff.h"
#include "gui_ballEditor.h"
#include "Wrench.h"
class Box;
class Ball;
class Item;
class Spear;
class Sword;
class Bow;
class Scythe;
class Mace;
class Staff;
class Wrench;

namespace gameWeaponOptions
{
    enum weaponType
    {
        spear,
        sword,
        Dagger,
        Shield,
        bowAndArrow,
        scythe,
        mace,
        staff,
        wrench,
    };
}
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
    Scythe scythe;
    Mace mace;
    Staff staff;
    Wrench wrench;
    std::vector<Weapon*> m_weapons;
    std::vector<Weapon*> m_weaponList = {&spear, &sword, &dagger, &shield, &bowAndArrow, &scythe, &mace, &staff, &wrench};
    std::string m_weaponListString;
    GuiBallEditorState m_guiBallEditorState;
    bool m_gameOver{ true };

    size_t m_weapon1{ 0 };
    size_t m_weapon2{ 1 };

    bool m_showMessageBox{ false };
public:
    void startUp();
    void update();
    void render();
    void shutDown();
};




