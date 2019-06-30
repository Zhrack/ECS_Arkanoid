#ifndef COMPONENT_LIST_H
#define COMPONENT_LIST_H

enum CompType {
    PLAYER_INPUT = 0,
    TRANSFORM,
    RENDER,
    BOX_COLLIDER,
    BALL,

    COUNT // keep last, to know number of components types
};

#endif // !COMPONENT_LIST_H
