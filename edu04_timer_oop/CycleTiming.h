#ifndef CYCLETIMING_H
#define CYCLETIMING_H
/*
 * CycleTiming.h
 *
 */
//#include <WProgram.h>
#include <Arduino.h>

class CycleTiming 
{
public:
    CycleTiming(int cycleTime);
    boolean check();

private:
    int cycleTimeMillis;

    unsigned long lastCheckMillis;
};

#endif


