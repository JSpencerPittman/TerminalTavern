#include <ncurses.h>

#include "Room.h"
#include "util.h"

#define PID 0

using namespace std;

Direction getInput(WINDOW* win) {
    int in_char = wgetch(win);

    switch (in_char) {
        case 'w':
            return UP;
        case 'd':
            return RIGHT;
        case 's':
            return DOWN;
        case 'a':
            return LEFT;
        default:
            return NONE;
    }
}

int main() {
    WINDOW *win = initscr();
    Room room(win);

    Player me(PID, 1, 2);
    room.addPlayer(me);

    Direction dir;

    while(1) {
        dir = getInput(win);

        if (dir == NONE) continue;

        room.movePlayer(PID, dir);
    }
}

