#include "CycleTiming.h"


CycleTiming::CycleTiming(int cycleTime) 
{
    cycleTimeMillis = cycleTime;
    lastCheckMillis = 0;
}


boolean CycleTiming::check() 
{
    if (lastCheckMillis == 0) 
    {
        lastCheckMillis = millis();
        return true;
    }
    else if ((millis() - lastCheckMillis) > cycleTimeMillis) 
    {
        lastCheckMillis = millis();
        return true;
    }
    else 
    {
        return false;
    }
}

