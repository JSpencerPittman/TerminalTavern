#include <boost/asio.hpp>
#include <iostream>
#include "Player.h"

using namespace std;
using namespace boost::asio;
using namespace boost::asio::ip;

// Driver program for recieving data from buffer
string getData(tcp::socket& socket) {
    boost::asio::streambuf buf;
    read_until(socket, buf, "\n");
    string data = buffer_cast<const char*>(buf.data());
    return data;
}

// Driver program to send data
void sendData(tcp::socket& socket, const string& message) {
    write(socket, buffer(message + '\n'));
}

int main() {
    PlayerMap pmap;

    cout << "Starting Server..." << endl;
    io_service io_serv;

    // Listen for any new incoming connection
    //  at port 9999 with IPv4 protocol
    tcp::acceptor acceptor_server(io_serv, tcp::endpoint(tcp::v4(), 9999));

    // Creating a socket object
    tcp::socket server_socket(io_serv);

    // Waiting for a connection
    cout << "Awaiting connection on port 9999..." << endl;
    acceptor_server.accept(server_socket);
    cout << "Connection established..." << endl;

    /*----- What to do once the connection is established -----*/

    // Give the user their ID
    int pID = pmap.availableID();

    if (pID == -1) {
        cout << "Can't accept user because of full lobby" << endl;
        sendData(server_socket, "Lobby is full");
        return 0;
    }
    sendData(server_socket, std::to_string(pID));

    // Waiting to Add Player to the server
    cout << "Pending Add Player Action..." << endl;
    Action addAction = Action::deserialize(getData(server_socket));
    if (addAction.getOperation() != Action::ADD) {
        cout << "Invalid action" << endl;
        sendData(server_socket, "Must be a Action::ADD Operation");
    }
    Player newPlayer{addAction.getPlayerID(), addAction.getX(), addAction.getY()};
    pmap.addPlayer(newPlayer);
    cout << "Player added to server..." << endl;

    // Main execution loop
    while (1) {
        cout << "Sending new player map..." << endl;
        sendData(server_socket, pmap.serialize());

        cout << "Waiting for user's action..." << endl;
        Action moveAction = Action::deserialize(getData(server_socket));

        cout << "Updating server player map..." << endl;
        pmap.movePlayer(moveAction.getPlayerID(), moveAction.getDirection());
    }

    /*---------------------------------------------------------*/

    return 0;
}