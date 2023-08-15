#include <boost/asio.hpp>
#include <iostream>
#include <vector>
#include "Player.h"
#include "Room.h"

using namespace std;
using namespace boost::asio;
using namespace boost::asio::ip;

string getData(tcp::socket& socket) {
    boost::asio::streambuf buf;
    read_until(socket, buf, "\n");
    string data = buffer_cast<const char*>(buf.data());
    data.pop_back(); // remove '\n'
    return data;
}

void sendData(tcp::socket& socket, const string& message) {
    write(socket, buffer(message + "\n"));
}

Direction getInput(WINDOW* win) {
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

int main(int argc, char* argv[]) {
     io_service io_serv;

     // Socket creation
     tcp::socket client_socket(io_serv);

     client_socket.connect(
             tcp::endpoint(
                     address::from_string("127.0.0.1"),
                     9999));

     cout << "Established a connection at 127.0.0.1:9999" << endl;

    /*----- What to do once the connection is established -----*/

    // Receive the Player ID
    cout << "Waiting for Player ID..." << endl;
    int pID = stoi(getData(client_socket));
    cout << "Player ID: #" << pID << endl;

    // establish local room
    WINDOW* win = initscr();
    Room room(win);
    room.addPlayer({pID, 5, 5});

    // Send the ADD Player Command
    Action addPlayer = Action::add(pID, 5, 5);
    sendData(client_socket, addPlayer.serialize());

    // Main execution loop
    Direction dir;
    while (1) {
        // Download server player map
        PlayerMap servPMAP = PlayerMap::deserialize(getData(client_socket));
        room.alignWithServer(servPMAP);

        // Move the user
        dir = getInput(win);
        room.movePlayer(pID, dir);

        // Send changes to server
        Action moveAction = Action::move(pID, dir);
        sendData(client_socket, moveAction.serialize());
    }

    /*---------------------------------------------------------*/

    return 0;
}