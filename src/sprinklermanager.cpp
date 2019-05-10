#include "sprinklermanager.hpp"

void SprinklerManager::start(uint8_t id, uint8_t time)
{
    if((id < MAX_SPRINKLER_COUNT) && (sprinklers[id] != nullptr))
    {
        sprinklers[id]->start(time);
    }
}

void SprinklerManager::stop()
{
    for(uint8_t i = 0; i < MAX_SPRINKLER_COUNT; i++)
    {
        if(sprinklers[i] != nullptr)
        {
            sprinklers[i]->stop();
        }
    }
}

void SprinklerManager::begin()
{
    sprinklerCount = 0;
}

void SprinklerManager::loop()
{
    for(uint8_t i = 0; i < MAX_SPRINKLER_COUNT; i++)
    {
        if(sprinklers[i] != nullptr)
        {
            if(sprinklers[i]->process())
            {
                break;
            }
        }
    }
}

void SprinklerManager::addSprinkler(Sprinkler* sprinkler)
{
    if((sprinkler != nullptr) && (sprinklerCount < 5))
    {
        uint8_t id = sprinkler->getId();

        if(id < MAX_SPRINKLER_COUNT)
        {
            sprinklers[id] = sprinkler;
            sprinklerCount++;
        }
    }
}