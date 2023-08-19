#ifndef TERMINALTAVERN_ACTION_H
#define TERMINALTAVERN_ACTION_H

#include <nlohmann/json.hpp>
#include <string>

#include "util.h"

class Action {
public:
    enum Operation { MOVE, ADD, DEL, REQID, REFRESH, NONE };

    static Action move(int pID, Direction dir);
    static Action add(int pID, int x, int y);
    static Action del(int pID);
    static Action reqID();
    static Action refresh();
    static Action none();

    Operation getOperation() const;
    int getPlayerID() const;
    Direction getDirection() const;
    int getX() const;
    int getY() const;


    std::string serialize() const;
    static Action deserialize(const std::string &serAct);
private:
    Action(Operation op, int pID, Direction dir, int x, int y);

    Operation op;
    Direction dir;
    int pID;
    int x, y;
};


#endif
