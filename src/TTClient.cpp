#include "TTClient.h"
#include "Action.h"

TTClient::TTClient(boost::asio::io_service& ioService)
    : socket_(tcp::socket(ioService)), playerID_(-1), refCnt_(0),
        timer_(ioService, boost::asio::chrono::milliseconds(100)) {}

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

    timer_.async_wait(boost::bind(
            &TTClient::refreshClient,
            this, room, win));
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
//    std::cout << "DATA_IN: " << data << std::endl;
    return data;
}

void TTClient::sendAction(const Action& action) {
    std::string serialAction = action.serialize();
//    std::cout << "DATA_OUT: " << serialAction << std::endl;
    write(socket_, boost::asio::buffer(serialAction + '\n'));
}

Direction TTClient::getInput(WINDOW* win) {
    int in_char = getch();

    switch (in_char) {
        case 'w':
            return UP;
        case 'd':
            return RIGHT;
        case 's':
            return DOWN;
        case 'a':
            return LEFT;
        case 'q':
            return LEAVE;
        default:
            return NONE;
    }
}

void TTClient::refreshClient(Room &room, WINDOW* win) {
    Direction dir = getInput(win);

    if(dir == LEAVE) {
        Action delPlayer = Action::del(playerID_);
        sendAction(delPlayer);
        return;
    } else if(dir != NONE) {
        room.movePlayer(playerID_, dir);
        Action moveAction = Action::move(playerID_, dir);
        sendAction(moveAction);
    } else {
        Action reqRefresh = Action::refresh();
        sendAction(reqRefresh);
    }

    PlayerMap serverMap = retrievePlayerMap();
    room.alignWithServer(serverMap);

    if (refCnt_++ % 100 == 0) {
        refCnt_ = 0;
        room.redrawRoom();
    }

    timer_.expires_from_now(boost::asio::chrono::milliseconds (50));
    timer_.async_wait(boost::bind(
            &TTClient::refreshClient,
            this, room, win));
}