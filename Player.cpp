#include "Player.h"

Player::Player(int pID, Coord2D initPos): pID(pID), pos(initPos) {}

Player::Player(int pID, int x, int y): pID(pID), pos{x, y} {}

void Player::move(Direction dir) {
    switch (dir) {
        case UP:
            pos.y--;
            break;
        case RIGHT:
            pos.x++;
            break;
        case DOWN:
            pos.y++;
            break;
        case LEFT:
            pos.x--;
            break;
        default:
            break;
    }
}

void Player::teleport(Coord2D pos) {
    pos = pos;
}

void Player::teleport(int x, int y) {
    teleport({x,y});
}

Coord2D Player::getPos() const {
    return pos;
}

int Player::getPlayerID() const {
    return pID;
}

std::string Player::serialize() const {
    using nlohmann::json;
    json jsonPly;
    jsonPly["pID"] = pID;
    jsonPly["x"] = pos.x;
    jsonPly["y"] = pos.y;
    return jsonPly.dump();
}

Player Player::deserialize(const std::string &serPlayer) {
    using nlohmann::json;
    json jsonPly = json::parse(serPlayer);
    int pID = jsonPly["pID"];
    int x = jsonPly["x"];
    int y = jsonPly["y"];
    return { pID, x, y };
}

PlayerMap::PlayerMap() {
    for (int i = 0; i < PLAYER_LIMIT; i++) online[i] = false;
}

int PlayerMap::availableID() {
    for (int i = 0; i < PLAYER_LIMIT; i++)
        if ( !online[i] ) return i;
    return -1;
}

Player PlayerMap::getPlayer(int pID) {
    auto plyItr = locatePlayer(pID);
    return *plyItr;
}

Coord2D PlayerMap::getPlayerPos(int pID) {
    auto plyItr = locatePlayer(pID);
    return plyItr->getPos();
}

std::vector<int> PlayerMap::getPlayerIDS() {
    std::vector<int> ids;
    auto playerItr = players.begin();
    while (playerItr != players.end()) {
        ids.push_back(playerItr->getPlayerID());
        playerItr++;
    }
    std::sort(ids.begin(), ids.end());
    return ids;
}

void PlayerMap::addPlayer(Player p) {
    if ( online[p.getPlayerID()] ) return; // ID already taken

    players.push_back(p);
    online[p.getPlayerID()] = true;
}

void PlayerMap::addPlayer(Coord2D pos) {
    int pID = availableID();
    if ( pID == -1 ) return; // Server full

    players.emplace_back(pID, pos);
    online[pID] = true;
}

void PlayerMap::addPlayer(int x, int y) {
    int pID = availableID();
    if ( pID == -1 ) return; // Server full

    Coord2D pos{x, y};
    players.emplace_back(pID, pos);
    online[pID] = true;
}

void PlayerMap::removePlayer(int pID) {
    auto playerItr = locatePlayer(pID);
    if ( playerItr == players.end() ) return; // Player not online

    players.erase(playerItr);
    online[pID] = false;
}

void PlayerMap::movePlayer(int pID, Direction dir) {
    auto playerItr = locatePlayer(pID);
    playerItr->move(dir);
}

void PlayerMap::teleportPlayer(int pID, Coord2D pos) {
    auto playItr = locatePlayer(pID);
    playItr->teleport(pos);
}

void PlayerMap::teleportPlayer(int pID, int x, int y) {
    teleportPlayer(pID, Coord2D{x, y});
}

void PlayerMap::print() {
    auto plyItr = players.begin();
    while(plyItr != players.end()) {
        std::cout << "Player: " << plyItr->getPlayerID();
        std::cout << " at " << plyItr->getPos().x << ", " << plyItr->getPos().y;
        std::cout << std::endl;
        plyItr++;
    }
}

std::string PlayerMap::serialize() {
    using nlohmann::json;

    std::vector<std::string> serPlayers;

    auto playerItr = players.begin();
    for ( ; playerItr != players.end(); playerItr++ )
        serPlayers.push_back(playerItr->serialize());

    json jsonPlyMap;
    jsonPlyMap["map"] = serPlayers;
    return jsonPlyMap.dump();
}

PlayerMap PlayerMap::deserialize(const std::string& serPlyMap) {
    using nlohmann::json;

    json jsonPlyMap = json::parse(serPlyMap);
    std::vector<std::string> serPlayers = jsonPlyMap["map"];

    PlayerMap plyMap;

    auto serPlyItr = serPlayers.begin();
    while ( serPlyItr != serPlayers.end() ) {
        plyMap.addPlayer(Player::deserialize(*serPlyItr));
        serPlyItr++;
    }
    return plyMap;
}

PlayerItr PlayerMap::locatePlayer(int pID) {
    if ( !online[pID] ) return players.end(); // Player not online

    auto playerItr = players.begin();
    while( playerItr != players.end() &&
            playerItr->getPlayerID() != pID )
        playerItr++;

    if ( playerItr == players.end() ) {
        std::cerr << "Player is online but not in map!" << std::endl;
        return players.end();
    }

    return playerItr;
}