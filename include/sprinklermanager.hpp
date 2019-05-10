#if !defined(_SUPERVISOR_HPP)
#define _SUPERVISOR_HPP

#include <Arduino.h>
#include "sprinkler.hpp"

#define MAX_SPRINKLER_COUNT 5

class SprinklerManager
{
    public:
        void start(uint8_t id, uint8_t time);
        void stop();
        void begin();
        void loop();
        void addSprinkler(Sprinkler* sprinkler);

    private:
        Sprinkler* sprinklers[MAX_SPRINKLER_COUNT];
        uint8_t sprinklerCount;
};

#endif // _SUPERVISOR_HPP
