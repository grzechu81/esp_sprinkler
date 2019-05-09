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
        void start(uint8_t id, uint8_t time);
        void stop();
        void begin();
        void loop();
};

#endif // _SUPERVISOR_HPP
