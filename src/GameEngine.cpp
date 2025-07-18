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
}


float timeStep = 1.0f / 60.0f;
int subStepCount  = 4;
void GameEngine::update()
{
    b2World_Step(m_worldId, timeStep, subStepCount);
    // testItem.rotate(ball);
    spear.rotate(ball);
    ball.testStopTime();
    ball2.testStopTime();

    ball.keepMoving();
    ball2.keepMoving();
}

void GameEngine::render()
{
    ClearBackground(RAYWHITE);
    box.renderRect();
    ball.renderCircle();
    ball2.renderCircle();
    // testItem.render();
    spear.render();
    spear.handleCollision(ball2 );
}

void GameEngine::shutDown()
{
    b2DestroyWorld(m_worldId);
}
