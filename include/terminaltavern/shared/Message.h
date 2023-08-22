#ifndef TERMINALTAVERN_MESSAGE_H
#define TERMINALTAVERN_MESSAGE_H

#include <deque>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

using nlohmann::json;

class Message {
public:
    Message(std::string username, std::string content);

    std::string username() const;
    std::string content() const;

    json toJSON() const;
    static Message fromJSON(json jsonMessage);
private:
    std::string username_;
    std::string content_;
};

class MessageHistory {
public:
    MessageHistory(int capacity);
    MessageHistory(int capacity, std::deque<Message> history);

    const Message& at(int i) const;
    size_t size() const;

    void addMessage(Message m);

    std::string serialize() const;
    static MessageHistory deserialize(const std::string& serMessageHistory);

//    MessageHistory& operator=(const MessageHistory& history);
private:
    std::deque<Message> history_;
    int capacity_;
};


#endif
