#if !defined(_SUPERVISOR_HPP)
#define _SUPERVISOR_HPP

#include <Arduino.h>

enum Area 
{
    Area1 = 0,
    Area2
};

enum State
{
    Stop,
    Start
};

class Supervisor
{
    public:
        void setJob(uint8_t id, bool run, uint8_t time);
        void begin();
        void loop();
};

#endif // _SUPERVISOR_HPP
