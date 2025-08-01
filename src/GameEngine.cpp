//
// Created by Harshad on 7/17/2025.
//

#include "GameEngine.h"

#include <cassert>

void GameEngine::startUp()
{
    for (int i{ 0 }; i < m_weaponList.size(); ++i)
    {
        m_weaponListString += m_weaponList[i]->getName() + ';';
    }
    std::cout << m_weaponListString;
    InitAudioDevice();
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = b2Vec2(0.0f, 70.0f);
    b2SetLengthUnitsPerMeter(30.0f);
    m_worldId = b2CreateWorld(&worldDef);

    box.initBox(m_worldId);
    ball2.setPos(Vector2(400, 400));
    ball.initBallBox2d(m_worldId);
    ball2.initBallBox2d(m_worldId);

    ball.initFont();
    ball2.initFont();

    // chgna the weapons fighting
    m_weapons.push_back(m_weaponList[m_weapon1]);
    m_weapons.push_back(m_weaponList[m_weapon2]);

    m_weapons[0]->setDirection(-1);
    m_weapons[1]->setDirection(1);


    for (auto& w : m_weapons)
    {
        w->initTextures();
    }


    ball.initDefaultColor(RED);
    ball2.initDefaultColor(BLUE);

    m_guiBallEditorState = InitGuiBallEditor();
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
float freezeLifeTime{ 0.3 };

// Timer stuff for dagger
Timer debounceTimerDagger{ 0 };
float debounceLifeTimeDagger{ 0.3 };

// Timer stuff for parrying
Timer freezeParryTimer{ 0 };
float freezeLifeTimeParry{ 0.4 };
bool parryHappened = false;


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

    ball.keepMoving(m_gameFrozen);
    ball2.keepMoving(m_gameFrozen);



    if (!m_gameOver)
    {
        m_weapons[0]->handleCollision(ball2, debounceTimerItem1, debounceLifeTimeItem1, ball, freezeTimer, freezeLifeTime, m_weapons[1]->getOrbitSpeed(), m_weapons[1]->getRect(), m_weapons[1]->getNormalOrbitSpeed(), m_weapons[1]->getFrozen(), m_gameFrozen, m_weapons[1]->getDirection(), m_weapons[1]->getCollDb(), m_weapons[1]->getAngle(), parryHappened);
        m_weapons[1]->handleCollision(ball, debounceTimerItem1, debounceLifeTimeItem1, ball2, freezeTimer, freezeLifeTime, m_weapons[0]->getOrbitSpeed(), m_weapons[0]->getRect(), m_weapons[0]->getNormalOrbitSpeed(), m_weapons[0]->getFrozen(), m_gameFrozen, m_weapons[0]->getDirection(), m_weapons[0]->getCollDb(), m_weapons[0]->getAngle(), parryHappened);
    }

    if (ball.getHealth() <= 0 || ball2.getHealth() <= 0)
    {
        m_gameOver = true;
    }

    if (IsKeyPressed(KEY_A))
        m_gameOver = true;
    if (IsKeyPressed(KEY_D))
        m_gameOver = false;
}

void GameEngine::render()
{
    ClearBackground(RAYWHITE);

    ball.setDefaultColorR(m_guiBallEditorState.ColorPicker005Value.r);
    ball.setDefaultColorG(m_guiBallEditorState.ColorPicker005Value.g);
    ball.setDefaultColorB(m_guiBallEditorState.ColorPicker005Value.b);

    ball2.setDefaultColorR(m_guiBallEditorState.ColorPicker006Value.r);
    ball2.setDefaultColorG(m_guiBallEditorState.ColorPicker006Value.g);
    ball2.setDefaultColorB(m_guiBallEditorState.ColorPicker006Value.b);
    if (!m_gameOver)
    {
        box.renderRect();
        // testItem.render();

        m_weapons[0]->render();
        ball.renderCircle();

        m_weapons[1]->render();
        ball2.renderCircle();
    }

    if (m_gameOver)
    {
        int buttonWidth = 100;
        int buttonHeight = 30;

        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        Rectangle buttonRect = {
            (screenWidth - buttonWidth) / 2.0f,
            (screenHeight - buttonHeight) / 2.0f,
            (float)buttonWidth,
            (float)buttonHeight
        };

        // if (GuiButton(buttonRect, "Start Button")) {
        //     ball.setHealth(100);
        //     ball2.setHealth(100);
        //     m_weapons[0]->resetState();
        //     m_weapons[1]->resetState();
        //     m_gameOver = false;
        // }

        GuiBallEditor(&m_guiBallEditorState, m_weaponListString.c_str());
        if (!m_guiBallEditorState.WindowBox000Active)
        {
            ball.setHealth(m_guiBallEditorState.Spinner003Value);
            ball2.setHealth(m_guiBallEditorState.Spinner004Value);

            m_weapons[0]->resetState();
            m_weapons[1]->resetState();

            m_weapons.clear();

            m_weapon1 = m_guiBallEditorState.ComboBox010Active;
            m_weapon2 = m_guiBallEditorState.ComboBox012Active;

            m_weapons.push_back(m_weaponList[m_weapon1]);
            m_weapons.push_back(m_weaponList[m_weapon2]);

            for (auto& w : m_weapons)
            {
                w->initTextures();
            }

            m_gameOver = false;
            m_guiBallEditorState.WindowBox000Active = true;
        }

    }
}

void GameEngine::shutDown()
{
    b2DestroyWorld(m_worldId);
}

