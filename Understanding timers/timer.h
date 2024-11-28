typedef struct
{
    float Lifetime;
}Timer;

// start or restart a timer with a specific lifetime
void StartTimer(Timer* timer, float duration)
{   // -> is to access elements in structs using pointers. we are accessing lifetime, changing it to duration
    timer->Lifetime = duration;  
}

// update a timer with the current frame time
void UpdateTimer(Timer* timer)
{
    // subtract this frame from the timer if it's not already expired
    if (timer->Lifetime > 0) 
    {
        timer->Lifetime -= GetFrameTime();
    }
}

// check if a timer is done.
bool TimerDone(Timer* timer)
{
    if (timer != NULL) 
    {
        return timer->Lifetime <= 0;  // return true if Lifetime is less than or equal to 0. return false if it is greater
    }

	return false;
}
