#include "TTClient.h"
#include "Action.h"

TTClient::TTClient(boost::asio::io_service& ioService)
    : socket_(tcp::socket(ioService)), playerID_(-1) {}

void TTClient::run() {
    // Establish connection
    std::cout << "Connecting to client" << std::endl;
    socket_.connect(
            tcp::endpoint(
                    boost::asio::ip::address::from_string("127.0.0.1"),
                    9999));


    // Get the player ID
    std::cout << "Requesting available player id..." << std::endl;
    Action requestID = Action::reqID();
    sendAction(requestID);
    playerID_ = retrievePlayerID();

    // Add the player to server
    std::cout << "Registering as player #" << playerID_ << std::endl;
    Action addPlayer = Action::add(playerID_, 5, 5);
    sendAction(addPlayer);

    // Intialize room
    PlayerMap servMap = retrievePlayerMap();
    WINDOW* win = initscr();
    Room room(win);
    room.alignWithServer(servMap);

    // Main execution loop
    Direction dir;
    while (1) {
        dir = getInput(win);
        room.movePlayer(playerID_, dir);
        Action moveAction = Action::move(playerID_, dir);
        sendAction(moveAction);
        servMap = retrievePlayerMap();
        room.alignWithServer(servMap);
    }
}

int TTClient::retrievePlayerID() {
    std::string serialPlayerID = getData();
    return std::stoi(serialPlayerID);
}

PlayerMap TTClient::retrievePlayerMap() {
    std::string serialPlayerMap = getData();
    return PlayerMap::deserialize(serialPlayerMap);
}

std::string TTClient::getData() {
    boost::asio::streambuf buf;
    boost::asio::read_until(socket_, buf, '\n');
    std::string data = boost::asio::buffer_cast<const char*>(buf.data());
    data.pop_back(); // Remove '\n'
    return data;
}

void TTClient::sendAction(const Action& action) {
    std::string serialAction = action.serialize();
    write(socket_, boost::asio::buffer(serialAction + '\n'));
}

Direction TTClient::getInput(WINDOW* win) {
    int in_char = wgetch(win);

    switch (in_char) {
        case 'w':
            return UP;
        case 'd':
            return RIGHT;
        case 's':
            return DOWN;
        case 'a':
            return LEFT;
        default:
            return NONE;
    }
}