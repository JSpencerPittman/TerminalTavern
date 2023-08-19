#include "Player.h"

#include <utility>

PlayerPixelMap::PlayerPixelMap(PlayerPixelMap::PixelMap pmap): pmap_(std::move(pmap)) {}

char PlayerPixelMap::getPixel(Coord2D pos) const {
    return pmap_[pos.y][pos.x];
}

char PlayerPixelMap::getPixel(int x, int y) const {
    return getPixel({x, y});
}

const PlayerPixelMap::PixelMap& PlayerPixelMap::getPixelMap() const {
    return pmap_;
}

PlayerPixelMap::Unravel PlayerPixelMap::unravelPixelMap() const {
    Unravel pmapUnr;

    for(int r = 0; r < 3; r++) {
        for(int c = 0; c < 3; c++) {
            Coord2D pos{c, r};
            char pixel = getPixel(pos);
            pmapUnr.emplace_back(pos, pixel);
        }
    }

    return pmapUnr;
}

PlayerPixelMap::Unravel
PlayerPixelMap::unravelPixelMapAtLocation(Coord2D pos) const {
    Unravel pmapUnr = unravelPixelMap();
    auto pmapUnrItr = pmapUnr.begin();
    for(; pmapUnrItr != pmapUnr.end(); ++pmapUnrItr) {
        pmapUnrItr->first.x += pos.x - 1;
        pmapUnrItr->first.y += pos.y - 1;
    }

    return pmapUnr;
}

PlayerPixelMap::Unravel
PlayerPixelMap::unravelPixelMapAtLocation(int x, int y) const {
        return unravelPixelMapAtLocation({x, y});
}

std::string PlayerPixelMap::serialize() const {
    std::string res;

    auto rowItr = pmap_.begin();
    for(; rowItr != pmap_.end(); ++rowItr) {
        auto colItr = rowItr->begin();
        for(; colItr != rowItr->end(); ++colItr)
            res.push_back(*colItr);
    }

    return res;
}

PlayerPixelMap PlayerPixelMap::deserialize(const std::string &serPixMap) {
    auto serItr = serPixMap.begin();
    PlayerPixelMap::PixelMap pixmap;
    for(int r = 0; r < 3; r++) {
        std::vector<char> row;
        for(int c = 0; c < 3; c++) {
            row.push_back(*serItr);
            serItr++;
        }
        pixmap.push_back(row);
    }

    return PlayerPixelMap(pixmap);
}

Player::Player(int pID, Coord2D initPos, PlayerPixelMap& pixelMap)
    : pID(pID), pos(initPos), pixmap(pixelMap) {}

Player::Player(int pID, int x, int y, PlayerPixelMap& pixelMap)
    : pID(pID), pos{x, y}, pixmap(pixelMap) {}

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

void Player::teleport(Coord2D p) {
    this->pos = p;
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

const PlayerPixelMap &Player::getPixelMap() const {
    return pixmap;
}

nlohmann::json Player::toJSON() const {
    using nlohmann::json;
    json jsonPlayer;
    jsonPlayer["pID"] = pID;
    jsonPlayer["x"] = pos.x;
    jsonPlayer["y"] = pos.y;
    jsonPlayer["pixmap"] = pixmap.serialize();
    return jsonPlayer;
}

Player Player::fromJSON(const nlohmann::json& jsonPlayer) {
    using nlohmann::json;
    int pID = jsonPlayer["pID"];
    int x = jsonPlayer["x"];
    int y = jsonPlayer["y"];
    PlayerPixelMap pixmap =
            PlayerPixelMap::deserialize(jsonPlayer["pixmap"]);
    return {pID, x, y, pixmap};
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
    PlayerPixelMap::PixelMap pixelmap = {
            {'0', '0', '0'},
            {'0', ' ', '0'},
            {'0', '0', '0'}
    };
    PlayerPixelMap playerPixelMap(pixelmap);

    players.emplace_back(pID, pos, playerPixelMap);
    online[pID] = true;
}

void PlayerMap::addPlayer(int x, int y) {
    addPlayer({x, y});
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

    std::vector<json> jsonPlayers;

    auto playerItr = players.begin();
    for ( ; playerItr != players.end(); playerItr++ )
        jsonPlayers.push_back(playerItr->toJSON());

    json jsonPlyMap;
    jsonPlyMap["map"] = jsonPlayers;
    return jsonPlyMap.dump();
}

PlayerMap PlayerMap::deserialize(const std::string& serPlyMap) {
    using nlohmann::json;

    json jsonPlyMap = json::parse(serPlyMap);
    std::vector<json> jsonPlayers = jsonPlyMap["map"];

    PlayerMap plyMap;

    auto jsonPlyItr = jsonPlayers.begin();
    while ( jsonPlyItr != jsonPlayers.end() ) {
        plyMap.addPlayer(Player::fromJSON(*jsonPlyItr));
        jsonPlyItr++;
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