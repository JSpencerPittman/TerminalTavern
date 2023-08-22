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
        case REFRESHROOM:
            return RefreshRoomPacket::fromJSON(jsonPacket);
        case MESSAGE:
            return SendMessagePacket::fromJSON(jsonPacket);
        case REFRESHCHAT:
            return RefreshChatPacket::fromJSON(jsonPacket);
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
        case REFRESHROOM:
            return RefreshRoomPacket::fromJSON(jsonPacket);
        case MESSAGE:
            return SendMessagePacket::fromJSON(jsonPacket);
        case REFRESHCHAT:
            return RefreshChatPacket::fromJSON(jsonPacket);
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
/*         Refresh Room Packet           */
/*---------------------------------------*/

RefreshRoomPacket::RefreshRoomPacket()
        : Packet(REFRESHROOM) {}

std::string RefreshRoomPacket::serialize() const {
    json jsonPacket;
    jsonPacket["operation"] = operation_;
    return jsonPacket.dump();
}

RefreshRoomPacket* RefreshRoomPacket::deserialize(const std::string& serPacket) {
    json jsonPacket = json::parse(serPacket);
    return fromJSON(jsonPacket);
}

RefreshRoomPacket* RefreshRoomPacket::fromJSON(json jsonPacket) {
    // Verify correct packet
    Operation operation = jsonPacket["operation"];
    if(operation != REFRESHROOM) return new RefreshRoomPacket{};

    return new RefreshRoomPacket{};
}

/*---------------------------------------*/
/*         Send Message Packet           */
/*---------------------------------------*/

SendMessagePacket::SendMessagePacket(std::string username, std::string message)
        : Packet(MESSAGE), username_(username), message_(message) {}

std::string SendMessagePacket::username() const { return username_; }

std::string SendMessagePacket::message() const { return message_; }

std::string SendMessagePacket::serialize() const {
    json jsonPacket;
    jsonPacket["operation"] = operation_;
    jsonPacket["username"] = username_;
    jsonPacket["message"] = message_;
    return jsonPacket.dump();
}

SendMessagePacket* SendMessagePacket::deserialize(const std::string& serPacket) {
    json jsonPacket = json::parse(serPacket);
    return fromJSON(jsonPacket);
}

SendMessagePacket* SendMessagePacket::fromJSON(json jsonPacket) {
    // Verify correct packet
    Operation operation = jsonPacket["operation"];
    if(operation != MESSAGE) return new SendMessagePacket{"", ""};

    std::string username = jsonPacket["username"];
    std::string message = jsonPacket["message"];
    return new SendMessagePacket{username, message};
}

/*---------------------------------------*/
/*         Refresh Room Packet           */
/*---------------------------------------*/

RefreshChatPacket::RefreshChatPacket()
        : Packet(REFRESHCHAT) {}

std::string RefreshChatPacket::serialize() const {
    json jsonPacket;
    jsonPacket["operation"] = operation_;
    return jsonPacket.dump();
}

RefreshChatPacket* RefreshChatPacket::deserialize(const std::string& serPacket) {
    json jsonPacket = json::parse(serPacket);
    return fromJSON(REFRESHCHAT);
}

RefreshChatPacket* RefreshChatPacket::fromJSON(json jsonPacket) {
    // Verify correct packet
    Operation operation = jsonPacket["operation"];
    if(operation != REFRESHCHAT) return new RefreshChatPacket{};

    return new RefreshChatPacket{};
}
