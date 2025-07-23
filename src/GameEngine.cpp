//
// Created by Harshad on 7/17/2025.
//

#include "GameEngine.h"

void GameEngine::startUp()
{
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = b2Vec2(0.0f, 50.0f);
    b2SetLengthUnitsPerMeter(30.0f);
    m_worldId = b2CreateWorld(&worldDef);

    box.initBox(m_worldId);
    ball.initBallBox2d(m_worldId);
    ball2.setPos(Vector2(400, 400));
    ball2.initBallBox2d(m_worldId);

    spear.initTextures();
    sword.initTextures();
}


float timeStep = 1.0f / 60.0f;
int subStepCount  = 4;

// Timer stuff for spear
Timer debounceTimerSpear{ 0 };
float debounceLifeTimeSpear{ 1 };

Timer freezeTimerSpear{ 0 };
float freezeLifeTimeSpear{ 0.9 };

// Timer stuff for sword
Timer debounceTimerSword{ 0 };
float debounceLifeTimeSword{ 0.7 };

Timer freezeTimerSword{ 0 };
float freezeLifeTimeSword{ 0.6 };


void GameEngine::update()
{
    b2World_Step(m_worldId, timeStep, subStepCount);
    // testItem.rotate(ball);
    spear.rotate(ball, spear.returnItemRect(), spear.getOrbitSpeed(), spear.getAngle(), spear.getDrawAngle(), spear.getWidth(), spear.getHeight(), spear.getRadiusOffset(), spear.getDirection());
    spear.rotateHead();
    sword.rotate(ball2, sword.getRect(), sword.getOrbitSpeed(), sword.getAngle(), sword.getDrawAngle(), sword.getWidth(), sword.getHeight(), sword.getRadiusOffset(), sword.getDirection());
    ball.testStopTime();
    ball2.testStopTime();

    ball.keepMoving();
    ball2.keepMoving();

    spear.handleCollision(ball2, debounceTimerSpear, debounceLifeTimeSpear, ball, freezeTimerSpear, freezeLifeTimeSpear, sword.getOrbitSpeed(), sword.getRect(), sword.getNormalOrbitSpeed());
    sword.handleCollision(ball, debounceTimerSword, debounceLifeTimeSword, ball2, freezeTimerSword, freezeLifeTimeSword, spear.getOrbitSpeed(), spear.returnItemRect(), spear.getNormalOrbitSpeed());
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
