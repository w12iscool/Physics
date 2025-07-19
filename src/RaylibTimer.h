//
// Created by Harshad on 7/19/2025.
//

#ifndef RAYLIBTIMER_H
#define RAYLIBTIMER_H

#include <raylib.h>

typedef struct
{
    float Lifetime;
}Timer;

// start or restart a timer with a specific lifetime
inline void StartTimer(Timer* timer, float lifetime)
{
    if (timer != NULL)
        timer->Lifetime = lifetime;
}

// update a timer with the current frame time
inline void UpdateTimer(Timer* timer)
{
    // subtract this frame from the timer if it's not allready expired
    if (timer != NULL && timer->Lifetime > 0)
        timer->Lifetime -= GetFrameTime();
}

// check if a timer is done.
inline bool TimerDone(Timer* timer)
{
    if (timer != NULL)
        return timer->Lifetime <= 0;

    return false;
}


#endif //RAYLIBTIMER_H
