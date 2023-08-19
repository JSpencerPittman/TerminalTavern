#include "TTClient.h"

#include <utility>
#include "Action.h"

TTClient::TTClient(boost::asio::io_service& ioService, std::string hostname, boost::asio::ip::port_type port)
    : socket_(tcp::socket(ioService)), playerID_(-1), refCnt_(0), room_(initscr()), resolver_(ioService),
        hostname_(std::move(hostname)), port_(port),
        timer_(ioService, boost::asio::chrono::milliseconds(REFRESH_RATE)) {
}

void TTClient::run() {
    // Establish connection
    tcp::endpoint endpoint = resolveEndpoint();
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

tcp::endpoint TTClient::resolveEndpoint() {
    boost::asio::ip::address ipAddress;

    if(isIPAddress(hostname_)) ipAddress = boost::asio::ip::address::from_string(hostname_);
    else {
        tcp::resolver::query query(hostname_, "http");
        tcp::resolver::iterator endpointIterator = resolver_.resolve(query);
        tcp::endpoint endpoint = *endpointIterator;
        ipAddress = endpoint.address();
    }

    return {ipAddress, port_};
}

bool TTClient::isIPAddress(const std::string& s) {
    std::vector<int> dotIndices;
    for(int i = 0; i < (int)s.size(); i++)
        if(s[i] == '.') dotIndices.push_back(i);

    if(dotIndices.size() != 3) return false;

    int prev = -1;
    std::string segment;
    auto dotItr = dotIndices.begin();
    while(dotItr != dotIndices.end()) {
        segment = std::string(s.begin() + prev + 1, s.begin() + (*dotItr));
        if(!isNumber(segment)) return false;
        prev = *dotItr;
        dotItr++;
    }

    segment = std::string(s.begin() + prev+1, s.end());
    return isNumber(segment);
}

bool TTClient::isNumber(const std::string& s) {
    auto sItr = s.begin();
    while(sItr != s.end() && isdigit(*sItr)) ++sItr;
    return sItr == s.end() && !s.empty();
}