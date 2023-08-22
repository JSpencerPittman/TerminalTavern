#include "Message.h"


Message::Message(std::string username, std::string content)
    : username_(username), content_(content) {}


std::string Message::username() const { return username_; }

std::string Message::content() const { return content_; }

json Message::toJSON() const {
    json jsonMessage;
    jsonMessage["username"] = username_;
    jsonMessage["content"] = content_;
    return jsonMessage;
}

Message Message::fromJSON(json jsonMessage) {
    return {jsonMessage["username"], jsonMessage["content"]};
}

MessageHistory::MessageHistory(int capacity): capacity_(capacity) {}

MessageHistory::MessageHistory(int capacity, std::deque<Message> history)
    : capacity_(capacity), history_(history) {}

const Message &MessageHistory::at(int i) const { return history_[i]; }

size_t MessageHistory::size() const { return history_.size(); }

void MessageHistory::addMessage(Message m) {
    history_.push_back(m);
    if(history_.size() > capacity_)
        history_.pop_front();
}

std::string MessageHistory::serialize() const {
    json jsonHistory;

    std::vector<json> jsonMessages;
    auto itr = history_.begin();
    while(itr != history_.end())
        jsonMessages.push_back(itr++->toJSON());

    jsonHistory["history"] = jsonMessages;
    jsonHistory["capacity"] = capacity_;
    return jsonHistory.dump();
}

MessageHistory MessageHistory::deserialize(const std::string& serMessageHistory) {
    json jsonHistory= json::parse(serMessageHistory);
    std::vector<json> jsonMessages = jsonHistory["history"];
    int capacity = jsonHistory["capacity"];

    MessageHistory history(capacity);
    auto itr = jsonMessages.begin();
    while(itr != jsonMessages.end())
        history.addMessage(Message::fromJSON(*itr++));

    return history;
}

//MessageHistory& MessageHistory::operator=(const MessageHistory& newHistory) {
//    history_ = newHistory.history_;
//    capacity_ = newHistory.capacity_;
//}