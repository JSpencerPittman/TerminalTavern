#ifndef TERMINALTAVERN_PACKET_H
#define TERMINALTAVERN_PACKET_H

#include <string>

#include <nlohmann/json.hpp>

#include "util.h"
#include "Player.h"

using nlohmann::json;

class Packet {
public:
    enum Operation { MOVE, ADD, DEL, REQID, REFRESHROOM, REFRESHCHAT, MESSAGE };

    Packet(Operation operation);

    Operation operation() const;

    virtual std::string serialize() const = 0;
    static Packet* deserialize(const std::string& serPacket);
    static Packet* fromJSON(json jsonPacket);
protected:
    Operation operation_;
};

class MovePacket : public Packet {
public:
    MovePacket(Direction direction);

    Direction direction() const;

    std::string serialize() const;
    static MovePacket* deserialize(const std::string& serPacket);
    static MovePacket* fromJSON(json jsonPacket);
private:
    Direction direction_;
};

class AddPacket : public Packet  {
public:
    AddPacket(Player player);

    Player player() const;

    std::string serialize() const;
    static AddPacket* deserialize(const std::string& serPacket);
    static AddPacket* fromJSON(json jsonPacket);
private:
    Player player_;
};

class DeletePacket : public Packet {
public:
    DeletePacket();

    std::string serialize() const;
    static DeletePacket* deserialize(const std::string& serPacket);
    static DeletePacket* fromJSON(json jsonPacket);
};

class RequestIDPacket : public Packet {
public:
    RequestIDPacket();

    std::string serialize() const;
    static RequestIDPacket* deserialize(const std::string& serPacket);
    static RequestIDPacket* fromJSON(json jsonPacket);
};

class RefreshRoomPacket : public Packet {
public:
    RefreshRoomPacket();

    std::string serialize() const;
    static RefreshRoomPacket* deserialize(const std::string& serPacket);
    static RefreshRoomPacket* fromJSON(json jsonPacket);
};

class SendMessagePacket : public Packet {
public:
    SendMessagePacket(std::string message);

    std::string message() const;

    std::string serialize() const;
    static SendMessagePacket* deserialize(const std::string& serPacket);
    static SendMessagePacket* fromJSON(json jsonPacket);
private:
    std::string message_;
};

class RefreshChatPacket : public Packet {
public:
    RefreshChatPacket();

    std::string serialize() const;
    static RefreshChatPacket* deserialize(const std::string& serPacket);
    static RefreshChatPacket* fromJSON(json jsonPacket);
};

#endif //TERMINALTAVERN_PACKET_H
