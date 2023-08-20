#ifndef TERMINALTAVERN_PACKET_H
#define TERMINALTAVERN_PACKET_H

#include <string>

#include <nlohmann/json.hpp>

#include "util.h"
#include "Player.h"

using nlohmann::json;

class Packet {
public:
    enum Operation { MOVE, ADD, DEL, REQID, REFRESH };

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
    MovePacket(int playerID, Direction direction);

    int playerID() const;
    Direction direction() const;

    std::string serialize() const;
    static MovePacket* deserialize(const std::string& serPacket);
    static MovePacket* fromJSON(json jsonPacket);
private:
    int playerID_;
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
    DeletePacket(int playerID);

    int playerID() const;

    std::string serialize() const;
    static DeletePacket* deserialize(const std::string& serPacket);
    static DeletePacket* fromJSON(json jsonPacket);
private:
    int playerID_;
};

class RequestIDPacket : public Packet {
public:
    RequestIDPacket();

    std::string serialize() const;
    static RequestIDPacket* deserialize(const std::string& serPacket);
    static RequestIDPacket* fromJSON(json jsonPacket);
};

class RefreshPacket : public Packet {
public:
    RefreshPacket();

    std::string serialize() const;
    static RefreshPacket* deserialize(const std::string& serPacket);
    static RefreshPacket* fromJSON(json jsonPacket);
};

#endif //TERMINALTAVERN_PACKET_H
