#ifndef TERMINALTAVERN_PLAYER_H
#define TERMINALTAVERN_PLAYER_H

#include <iostream>
#include <vector>
#include <string>
#include "Action.h"
#include "util.h"

class Player {
public:
    Player(int pID, Coord2D initPos);
    Player(int pID, int x, int y);

    void move(Direction dir);
    void teleport(Coord2D pos);
    void teleport(int x, int y);

    Coord2D getPos() const;
    int getPlayerID() const;

    std::string serialize() const;
    static Player deserialize(const std::string &serPlayer);
private:
    int pID;
    Coord2D pos;
};

#define PLAYER_LIMIT 10
typedef std::vector<Player>::iterator PlayerItr;

class PlayerMap {
public:
    PlayerMap();

    int availableID();

    Player getPlayer(int pID);
    Coord2D getPlayerPos(int pID);
    std::vector<int> getPlayerIDS();

    void addPlayer(Player p);
    void addPlayer(Coord2D pos);
    void addPlayer(int x, int y);
    void removePlayer(int pID);

    void movePlayer(int pID, Direction dir);
    void teleportPlayer(int pID, Coord2D pos);
    void teleportPlayer(int pID, int x, int y);

    void print();

    std::string serialize();
    static PlayerMap deserialize(const std::string& serPlyMap);
private:
    PlayerItr locatePlayer(int pID);

    std::vector<Player> players;
    bool online[PLAYER_LIMIT];
};


#endif //TERMINALTAVERN_PLAYER_H
