#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace Constants {
    static int SCREEN_WIDTH = 1024;
    static int SCREEN_HEIGHT = 768;

    static int NUM_LIVES = 3;

    static float PADDLE_VELOCITY = 500.0f;
    static float PADDLE_SIZE_X = 80.0f;
    static float PADDLE_SIZE_Y = 30.0f;
}


using EntityID = unsigned long;


#endif // !CONSTANTS_H