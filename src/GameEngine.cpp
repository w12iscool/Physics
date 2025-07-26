//
// Created by Harshad on 7/17/2025.
//

#include "GameEngine.h"

#include <cassert>

void GameEngine::startUp()
{
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = b2Vec2(0.0f, 70.0f);
    b2SetLengthUnitsPerMeter(30.0f);
    m_worldId = b2CreateWorld(&worldDef);

    box.initBox(m_worldId);
    ball2.setPos(Vector2(400, 400));
    ball.initBallBox2d(m_worldId);
    ball2.initBallBox2d(m_worldId);

    // chgna the weapons fighting
    m_weapons.push_back(&shield);
    m_weapons.push_back(&dagger);

    for (auto& w : m_weapons)
    {
        w->initTextures();
    }

    ball.initDefaultColor(RED);
    ball2.initDefaultColor(BLUE);
}


float timeStep = 1.0f / 60.0f;
int subStepCount  = 4;

// Timer stuff for spear
Timer debounceTimerItem1{ 0 };
float debounceLifeTimeItem1{ 0.7 };

Timer randomMovementTimer{ 0 };
float randomMovementLifetime{ 5 };

// Timer stuff for sword
Timer debounceTimerItem2{ 0 };
float debounceLifeTimeItem2{ 0.7 };

Timer freezeTimer{ 0 };
float freezeLifeTime{ 0.6 };

// Timer stuff for dagger
Timer debounceTimerDagger{ 0 };
float debounceLifeTimeDagger{ 0.3 };


void GameEngine::update()
{
    b2World_Step(m_worldId, timeStep, subStepCount);
    // testItem.rotate(ball);
    float spearOrbitSpeed = spear.getOrbitSpeed();
    float spearNormalOrbitSpeed = spear.getNormalOrbitSpeed();

    float swordOrbitSpeed = sword.getOrbitSpeed();
    float swordNormalOrbitSpeed = sword.getNormalOrbitSpeed();

    float daggerOrbitSpeed = dagger.getOrbitSpeed();
    float daggerNormalOrbitSpeed = dagger.getNormalOrbitSpeed();

    if (m_gameFrozen)
    {
        ball.setFrozen(true);
        ball2.setFrozen(true);
    }
    else
    {
        ball.setFrozen(false);
        ball2.setFrozen(false);
    }

    ball.handleFreezing(m_weapons[0]->getOrbitSpeed(), m_weapons[0]->getNormalOrbitSpeed(), ball2, m_gameFrozen);
    ball2.handleFreezing(m_weapons[1]->getOrbitSpeed(), m_weapons[1]->getNormalOrbitSpeed(), ball, m_gameFrozen);

    // sword.rotate(ball, swordOrbitSpeed);
    spear.rotateHead();
    // dagger.rotate(ball2, daggerOrbitSpeed);

    m_weapons[0]->rotate(ball, m_weapons[0]->getOrbitSpeed());
    spear.rotateHead();
    m_weapons[1]->rotate(ball2, m_weapons[1]->getOrbitSpeed());

    ball.keepMoving();
    ball2.keepMoving();

    m_weapons[0]->handleCollision(ball2, debounceTimerItem1, debounceLifeTimeItem1, ball, freezeTimer, freezeLifeTime, m_weapons[1]->getOrbitSpeed(), m_weapons[1]->getRect(), m_weapons[1]->getNormalOrbitSpeed(), m_weapons[1]->getFrozen(), m_gameFrozen, m_weapons[1]->getDirection(), m_weapons[1]->getCollDb(), m_weapons[1]->getAngle());
    m_weapons[1]->handleCollision(ball, debounceTimerItem1, debounceLifeTimeItem1, ball2, freezeTimer, freezeLifeTime, m_weapons[0]->getOrbitSpeed(), m_weapons[0]->getRect(), m_weapons[0]->getNormalOrbitSpeed(), m_weapons[0]->getFrozen(), m_gameFrozen, m_weapons[0]->getDirection(), m_weapons[0]->getCollDb(), m_weapons[0]->getAngle());


}

void GameEngine::render()
{
    ClearBackground(RAYWHITE);
    box.renderRect();
    // testItem.render();
    if (ball.getHealth() > 0)
    {
        m_weapons[0]->render();
        ball.renderCircle();
    }
    else
    {
        m_weapons.erase(m_weapons.begin());
    }

    if (ball2.getHealth() > 0)
    {
        m_weapons[1]->render();
        ball2.renderCircle();
    }
    else
    {
        m_weapons.erase(m_weapons.end());
    }


}

void GameEngine::shutDown()
{
    b2DestroyWorld(m_worldId);
}
