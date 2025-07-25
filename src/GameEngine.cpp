//
// Created by Harshad on 7/17/2025.
//

#include "GameEngine.h"

#include <cassert>

void GameEngine::startUp()
{
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = b2Vec2(0.0f, 50.0f);
    b2SetLengthUnitsPerMeter(30.0f);
    m_worldId = b2CreateWorld(&worldDef);

    box.initBox(m_worldId);
    ball2.setPos(Vector2(400, 400));
    ball.initBallBox2d(m_worldId);
    ball2.initBallBox2d(m_worldId);

    spear.initTextures();
    sword.initTextures();
}


float timeStep = 1.0f / 60.0f;
int subStepCount  = 4;

// Timer stuff for spear
Timer debounceTimerSpear{ 0 };
float debounceLifeTimeSpear{ 0.7 };

Timer freezeTimerSpear{ 0 };
float freezeLifeTimeSpear{ 0.6 };

// Timer stuff for sword
Timer debounceTimerSword{ 0 };
float debounceLifeTimeSword{ 0.7 };

Timer freezeTimerSword{ 0 };
float freezeLifeTimeSword{ 0.6 };


void GameEngine::update()
{
    b2World_Step(m_worldId, timeStep, subStepCount);
    // testItem.rotate(ball);
    float spearOrbitSpeed = spear.getOrbitSpeed();
    float spearNormalOrbitSpeed = spear.getNormalOrbitSpeed();

    float swordOrbitSpeed = sword.getOrbitSpeed();
    float swordNormalOrbitSpeed = sword.getNormalOrbitSpeed();

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

    ball.handleFreezing(spearOrbitSpeed, spearNormalOrbitSpeed, ball2, m_gameFrozen);
    ball2.handleFreezing(swordOrbitSpeed, swordNormalOrbitSpeed, ball, m_gameFrozen);

    spear.rotate(ball, spear.returnItemRect(), spearOrbitSpeed, spear.getAngle(), spear.getDrawAngle(), spear.getWidth(), spear.getHeight(), spear.getRadiusOffset(), spear.getDirection(), spear.getIsFrozen());
    spear.rotateHead();
    sword.rotate(ball2, sword.getRect(), swordOrbitSpeed, sword.getAngle(), sword.getDrawAngle(), sword.getWidth(), sword.getHeight(), sword.getRadiusOffset(), sword.getDirection(), sword.getFrozen());

    ball.keepMoving();
    ball2.keepMoving();

    spear.handleCollision(ball2, debounceTimerSpear, debounceLifeTimeSpear, ball, freezeTimerSword, freezeLifeTimeSword, sword.getOrbitSpeed(), sword.getRect(), sword.getNormalOrbitSpeed(), sword.getFrozen(), m_gameFrozen);
    sword.handleCollision(ball, debounceTimerSword, debounceLifeTimeSword, ball2, freezeTimerSword, freezeLifeTimeSword, spear.getOrbitSpeed(), spear.returnItemRect(), spear.getNormalOrbitSpeed(), spear.getIsFrozen(), m_gameFrozen);


}

void GameEngine::render()
{
    ClearBackground(RAYWHITE);
    box.renderRect();
    ball.renderCircle();
    ball2.renderCircle();
    // testItem.render();
    spear.render();
    sword.render();

}

void GameEngine::shutDown()
{
    b2DestroyWorld(m_worldId);
}
