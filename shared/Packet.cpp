#include "Packet.h"

#include <utility>

/*---------------------------------------*/
/*                Packet                 */
/*---------------------------------------*/

#include "Packet.h"

Packet::Packet(Operation operation) : operation_(operation) {}

Packet::Operation Packet::operation() const {return operation_;}

Packet* Packet::deserialize(const std::string& serPacket) {
    json jsonPacket = json::parse(serPacket);
    Operation operation = jsonPacket["operation"];

    switch (operation) {
        case MOVE:
            return MovePacket::fromJSON(jsonPacket);
        case ADD:
            return AddPacket::fromJSON(jsonPacket);
        case DEL:
            return DeletePacket::fromJSON(jsonPacket);
        case REQID:
            return RequestIDPacket::fromJSON(jsonPacket);
        case REFRESH:
            return RefreshPacket::fromJSON(jsonPacket);
    }

    return nullptr;
}

Packet* Packet::fromJSON(json jsonPacket) {
    Operation operation = jsonPacket["operation"];

    switch (operation) {
        case MOVE:
            return MovePacket::fromJSON(jsonPacket);
        case ADD:
            return AddPacket::fromJSON(jsonPacket);
        case DEL:
            return DeletePacket::fromJSON(jsonPacket);
        case REQID:
            return RequestIDPacket::fromJSON(jsonPacket);
        case REFRESH:
            return RefreshPacket::fromJSON(jsonPacket);
    }

    return nullptr;
}


/*---------------------------------------*/
/*              Move Packet              */
/*---------------------------------------*/

MovePacket::MovePacket(int playerID, Direction direction)
    : Packet(MOVE), playerID_(playerID), direction_(direction) {}

int MovePacket::playerID() const { return playerID_; }
Direction MovePacket::direction() const { return direction_; }

std::string MovePacket::serialize() const {
    json jsonPacket;
    jsonPacket["operation"] = operation_;
    jsonPacket["playerID"] = playerID_;
    jsonPacket["direction"] = direction_;
    return jsonPacket.dump();
}

MovePacket* MovePacket::deserialize(const std::string& serPacket) {
    json jsonPacket = json::parse(serPacket);
    return fromJSON(jsonPacket);
}

MovePacket* MovePacket::fromJSON(json jsonPacket) {
    // Verify correct packet
    Operation operation = jsonPacket["operation"];
    if(operation != MOVE) return new MovePacket{-1, NONE};

    int playerID = jsonPacket["playerID"];
    Direction direction = jsonPacket["direction"];

    return new MovePacket{playerID, direction};
}

/*---------------------------------------*/
/*              Add Packet              */
/*---------------------------------------*/

AddPacket::AddPacket(Player player)
        : Packet(ADD), player_(std::move(player)) {}

Player AddPacket::player() const { return player_; }

std::string AddPacket::serialize() const {
    json jsonPacket;
    jsonPacket["operation"] = operation_;
    jsonPacket["player"] = player_.toJSON();
    return jsonPacket.dump();
}

AddPacket* AddPacket::deserialize(const std::string& serPacket) {
    json jsonPacket = json::parse(serPacket);
    return fromJSON(jsonPacket);
}

AddPacket* AddPacket::fromJSON(json jsonPacket) {
    // Verify correct packet
    Operation operation = jsonPacket["operation"];
    if(operation != ADD) {
        Player nullPlayer {-1, -1, -1, PlayerPixelMap(), ""};
        return new AddPacket{nullPlayer};
    }

    Player player = Player::fromJSON(jsonPacket["player"]);

    return new AddPacket{player};
}

/*---------------------------------------*/
/*             Delete Packet             */
/*---------------------------------------*/

DeletePacket::DeletePacket(int playerID)
        : Packet(DEL), playerID_(playerID) {}

int DeletePacket::playerID() const { return playerID_; }

std::string DeletePacket::serialize() const {
    json jsonPacket;
    jsonPacket["operation"] = operation_;
    jsonPacket["playerID"] = playerID_;
    return jsonPacket.dump();
}

DeletePacket* DeletePacket::deserialize(const std::string& serPacket) {
    json jsonPacket = json::parse(serPacket);
    return fromJSON(jsonPacket);
}

DeletePacket* DeletePacket::fromJSON(json jsonPacket) {
    // Verify correct packet
    Operation operation = jsonPacket["operation"];
    if(operation != DEL) return new DeletePacket{-1};

    int playerID = jsonPacket["playerID"];

    return new DeletePacket{playerID};
}

/*---------------------------------------*/
/*           Request ID Packet           */
/*---------------------------------------*/

RequestIDPacket::RequestIDPacket()
        : Packet(REQID) {}

std::string RequestIDPacket::serialize() const {
    json jsonPacket;
    jsonPacket["operation"] = operation_;
    return jsonPacket.dump();
}

RequestIDPacket* RequestIDPacket::deserialize(const std::string& serPacket) {
    json jsonPacket = json::parse(serPacket);
    return fromJSON(jsonPacket);
}

RequestIDPacket* RequestIDPacket::fromJSON(json jsonPacket) {
    // Verify correct packet
    Operation operation = jsonPacket["operation"];
    if(operation != REQID) return new RequestIDPacket{};

    return new RequestIDPacket{};
}

/*---------------------------------------*/
/*            Refresh Packet             */
/*---------------------------------------*/

RefreshPacket::RefreshPacket()
        : Packet(REFRESH) {}

std::string RefreshPacket::serialize() const {
    json jsonPacket;
    jsonPacket["operation"] = operation_;
    return jsonPacket.dump();
}

RefreshPacket* RefreshPacket::deserialize(const std::string& serPacket) {
    json jsonPacket = json::parse(serPacket);
    return fromJSON(jsonPacket);
}

RefreshPacket* RefreshPacket::fromJSON(json jsonPacket) {
    // Verify correct packet
    Operation operation = jsonPacket["operation"];
    if(operation != REFRESH) return new RefreshPacket{};

    return new RefreshPacket{};
}

