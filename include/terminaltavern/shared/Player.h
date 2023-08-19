#ifndef TERMINALTAVERN_PLAYER_H
#define TERMINALTAVERN_PLAYER_H

#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include "Action.h"
#include "util.h"

class PlayerPixelMap {
public:
    typedef std::vector<std::vector<char>> PixelMap;
    typedef std::vector<std::pair<Coord2D, char>> Unravel;

    explicit PlayerPixelMap(PixelMap pmap);

    char getPixel(Coord2D pos) const;
    char getPixel(int x, int y) const;

    const PixelMap& getPixelMap() const;

    Unravel unravelPixelMap() const;

    Unravel unravelPixelMapAtLocation(Coord2D pos) const;
    Unravel unravelPixelMapAtLocation(int x, int y) const;

    std::string serialize() const;
    static PlayerPixelMap deserialize(const std::string& serPixMap);
private:
    PixelMap pmap_;
};

class Player {
public:
    Player(int pID, Coord2D initPos, PlayerPixelMap& pixelMap);
    Player(int pID, int x, int y, PlayerPixelMap& pixelMap);

    void move(Direction dir);
    void teleport(Coord2D pos);
    void teleport(int x, int y);

    Coord2D getPos() const;
    int getPlayerID() const;
    const PlayerPixelMap& getPixelMap() const;

    nlohmann::json toJSON() const;
    static Player fromJSON(const nlohmann::json& jsonPlayer);

private:
    int pID;
    Coord2D pos;
    PlayerPixelMap pixmap;
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
