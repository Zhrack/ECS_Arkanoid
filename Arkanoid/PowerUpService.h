#ifndef POWER_UP_SERVICE_H
#define POWER_UP_SERVICE_H

#include <random>

#include "SFML/Graphics.hpp"

class GameState;

enum PUType
{
    PU_STICKY = 0,

    PU_COUNT
};

class PowerUpService
{
public:
    PowerUpService(GameState* game);
    ~PowerUpService();

    /// <summary>
    /// A chance of randowmly spawning a powerup.
    /// </summary>
    void spawnRandomPU(const sf::Vector2f & pos);

private:
    void createStickyPU(const sf::Vector2f & pos);

private:
    GameState* mGame;

    std::default_random_engine mGenerator;
    std::uniform_int_distribution<int> mDistr;
};


#endif // !POWER_UP_SERVICE_H
