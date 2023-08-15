#include "Action.h"

#include <iostream>

Action Action::move(int pID, Direction dir) {
    return {MOVE, pID, dir, -1, -1};
}

Action Action::add(int pID, int x, int y) {
    return {ADD, pID, Direction::NONE, x, y};
}

Action Action::del(int pID) {
    return {DEL, pID, Direction::NONE, -1, -1};
}

Action Action::reqID() {
    return {REQID, -1, Direction::NONE, -1, -1};
}

Action::Action(Action::Operation op, int pID, Direction dir, int x, int y):
    op(op), pID(pID), dir(dir), x(x), y(y) {}

Action::Operation Action::getOperation() const {
    return op;
}

int Action::getPlayerID() const {
    return pID;
}

Direction Action::getDirection() const {
    return dir;
}

int Action::getX() const {
    return x;
}
int Action::getY() const {
    return y;
}

std::string Action::serialize() const {
    using nlohmann::json;
    json jsonAct;

    switch (op) {
        case REQID:
            jsonAct["op"] = REQID;
            break;
        case MOVE:
            jsonAct["op"] = MOVE;
            jsonAct["pID"] = pID;
            jsonAct["dir"] = dir;
            break;
        case ADD:
            jsonAct["op"] = ADD;
            jsonAct["pID"] = pID;
            jsonAct["x"] = x;
            jsonAct["y"] = y;
            break;
        case DEL:
            jsonAct["op"] = DEL;
            jsonAct["pID"] = pID;
            break;
        default:
            return "";
    }

    return jsonAct.dump();
}

Action Action::deserialize(const std::string &serAct) {
    using nlohmann::json;
    json jsonAct = json::parse(serAct);

    Operation op = jsonAct["op"];

    int pID, x, y;
    Direction dir;

    switch (op) {
        case REQID:
            return reqID();
        case ADD:
            x = jsonAct["x"];
            y = jsonAct["y"];
            pID = jsonAct["pID"];
            return add(pID, x, y);
        case MOVE:
            dir = (Direction)jsonAct["dir"];
            pID = jsonAct["pID"];
            return move(pID, dir);
        case DEL:
            pID = jsonAct["pID"];
            return del(pID);
        default:
            return Action::reqID();
    }
}

