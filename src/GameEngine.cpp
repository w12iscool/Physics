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

    spear.initTextures();
    sword.initTextures();
    dagger.initTextures();

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

    ball.handleFreezing(swordOrbitSpeed, swordNormalOrbitSpeed, ball2, m_gameFrozen);
    ball2.handleFreezing(daggerOrbitSpeed, daggerNormalOrbitSpeed, ball, m_gameFrozen);

    sword.rotate(ball, sword.getRect(), swordOrbitSpeed, sword.getAngle(), sword.getDrawAngle(), sword.getWidth(), sword.getHeight(), sword.getRadiusOffset(), sword.getDirection(), sword.getFrozen());
    spear.rotateHead();
    dagger.rotate(ball2, dagger.getRect(), daggerOrbitSpeed, dagger.getAngle(), dagger.getDrawAngle(), dagger.getWidth(), dagger.getHeight(), dagger.getRadiusOffset(), dagger.getDirection(), dagger.getFrozen());

    ball.keepMoving();
    ball2.keepMoving();

    sword.handleCollision(ball2, debounceTimerItem1, debounceLifeTimeItem1, ball, freezeTimer, freezeLifeTime, dagger.getOrbitSpeed(), dagger.getRect(), dagger.getNormalOrbitSpeed(), dagger.getFrozen(), m_gameFrozen, dagger.getDirection(), dagger.getCollDb(), dagger.getAngle());
    dagger.handleCollision(ball, debounceTimerDagger, debounceLifeTimeDagger, ball2, freezeTimer, freezeLifeTime, sword.getOrbitSpeed(), sword.getRect(), sword.getNormalOrbitSpeed(), sword.getFrozen(), m_gameFrozen, sword.getDirection(), sword.getCollDb(), sword.getAngle());
    // ball.handleColor(hitItem::itemHit::Spear);
    // ball2.handleColor(hitItem::itemHit::Sword);\


}

void GameEngine::render()
{
    ClearBackground(RAYWHITE);
    box.renderRect();
    // testItem.render();
    if (ball.getHealth() > 0)
    {
        sword.render();
        ball.renderCircle();

    }

    if (ball2.getHealth() > 0)
    {
        dagger.render();
        ball2.renderCircle();
    }


}

void GameEngine::shutDown()
{
    b2DestroyWorld(m_worldId);
}
