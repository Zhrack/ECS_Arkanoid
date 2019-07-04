#ifndef COMPONENT_LIST_H
#define COMPONENT_LIST_H

enum CompType {
    PADDLE_BEHAVIOR = 0,
    BALL_BEHAVIOR,
    BRICK,

    TRANSFORM,

    RECT_RENDER,
    CIRCLE_RENDER,
    SPRITE_RENDER,

    BOX_COLLIDER,
    CIRCLE_COLLIDER,

    STICKY,

    GAME_OVER_WATCHER,

    COUNT // keep last, to know number of components types
};

#endif // !COMPONENT_LIST_H
