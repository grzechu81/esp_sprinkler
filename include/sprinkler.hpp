#if !defined(_SPRINKLER_HPP)
#define _SPRINKLER_HPP

#include <Arduino.h>

class Sprinkler
{
    public:
        Sprinkler(uint8_t controlPin, uint8_t maxTime);

    public:
        void start(uint8_t time);
        void stop();
        void process();
    
    private:
        uint8_t pin;
        uint8_t maxWorkingTime;
};

#endif // _SPRINKLER_HPP
