#ifndef TERMINALTAVERN_UTIL_H
#define TERMINALTAVERN_UTIL_H

#include <string>

enum Direction { UP, RIGHT, DOWN, LEFT, NONE, LEAVE, SWITCH };

typedef struct {
    int x;
    int y;
} Coord2D;

typedef struct {
    std::string hostname;
    int port;
} ServerInfo;

#endif //TERMINALTAVERN_UTIL_H
