#include "sprinkler.hpp"

Sprinkler::Sprinkler(uint8_t id, uint8_t controlPin, uint8_t maxTime) : 
    id(id), pin(controlPin), scheduledTime(0), maxWorkingTime(maxTime), isStarted(false), startMilisecond(0)
{
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
}

uint8_t Sprinkler::getId()
{
    return id;
}

void Sprinkler::start(uint8_t time)
{
    if(time > maxWorkingTime)
    {
        scheduledTime = maxWorkingTime;
    }
    else
    {
        scheduledTime = time;
    }
    
    isStarted = true;
}

void Sprinkler::stop()
{
    digitalWrite(pin, LOW);
    isStarted = false;
}

bool Sprinkler::process()
{
    if(isStarted)
    {
        long currentMillis = 0;

        if(startMilisecond == 0)
        {
            startMilisecond = millis();
            currentMillis = startMilisecond;
        }
        else
        {
           currentMillis = millis();
        }
        
        if ((currentMillis - startMilisecond) < (scheduledTime * 1000))
        {
            digitalWrite(pin, HIGH);
        }
        else
        {
            digitalWrite(pin, LOW);
            isStarted = false;
        }
    }

    return isStarted;
}
