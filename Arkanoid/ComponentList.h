#ifndef COMPONENT_LIST_H
#define COMPONENT_LIST_H

enum CompType {
    PLAYER_INPUT = 0,
    BALL,

    TRANSFORM,

    RECT_RENDER,
    CIRCLE_RENDER,
    SPRITE_RENDER,

    BOX_COLLIDER,
    CIRCLE_COLLIDER,

    COUNT // keep last, to know number of components types
};

#endif // !COMPONENT_LIST_H
