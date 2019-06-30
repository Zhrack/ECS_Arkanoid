#ifndef CONSTANTS_H
#define CONSTANTS_H


using EntityID = unsigned long;

struct Constants {
    static const int SCREEN_WIDTH;
    static const int SCREEN_HEIGHT;

    static const int NUM_LIVES;

    static const float PADDLE_VELOCITY;
    static const float PADDLE_SIZE_X;
    static const float PADDLE_SIZE_Y;
};

#endif // !CONSTANTS_H