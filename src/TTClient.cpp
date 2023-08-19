#include "TTClient.h"
#include "Action.h"

TTClient::TTClient(boost::asio::io_service& ioService, WINDOW* win)
    : socket_(tcp::socket(ioService)), playerID_(-1), refCnt_(0), win_(win), room_(win), resolver_(ioService),
        timer_(ioService, boost::asio::chrono::milliseconds(REFRESH_RATE)) {
}

void TTClient::run() {
    // Establish connection
    tcp::resolver::query query(SERVER_HOSTNAME, "http");
    tcp::resolver::iterator endpointIterator = resolver_.resolve(query);
    tcp::endpoint endpoint = *endpointIterator;
    boost::asio::ip::address address = endpoint.address();

    endpoint.port(SERVER_PORT);

    socket_.connect(endpoint);

    // Get the player ID
    Action requestID = Action::reqID();
    sendAction(requestID);
    playerID_ = retrievePlayerID();

    // Add the player to server
    Action addPlayer = Action::add(playerID_, 5, 5);
    sendAction(addPlayer);

    // Initialize room
    PlayerMap servMap = retrievePlayerMap();
    room_.alignWithServer(servMap);

    timer_.async_wait(boost::bind(&TTClient::refreshClient, this));
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

Direction TTClient::getInput() {
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

void TTClient::refreshClient() {
    Direction dir = getInput();

    if(dir == LEAVE) {
        Action delPlayer = Action::del(playerID_);
        sendAction(delPlayer);
        return;
    } else if(dir != NONE) {
        room_.movePlayer(playerID_, dir);
        Action moveAction = Action::move(playerID_, dir);
        sendAction(moveAction);
    } else {
        Action reqRefresh = Action::refresh();
        sendAction(reqRefresh);
    }

    PlayerMap serverMap = retrievePlayerMap();
    room_.alignWithServer(serverMap);

    if (refCnt_++ % REDRAW_FREQUENCY == 0) {
        refCnt_ = 0;
        room_.redrawRoom();
    }

    timer_.expires_from_now(boost::asio::chrono::milliseconds (REFRESH_RATE));
    timer_.async_wait(boost::bind(&TTClient::refreshClient, this));
}