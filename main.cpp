#include "vpnclasses.h"
#include <iostream>
#include <string>


using namespace std;
int main() {
    // Create VPN server and client objects
    VPNServer server("127.0.0.1", 8080);
    VPNClient client("127.0.0.1", 8080);

    // Start the server in a separate thread
    std::thread serverThread(&VPNServer::connect, &server);
    serverThread.detach();

    // Give the server some time to start listening (optional)
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Connect the client to the server
    client.connect();

    // Send data from the client
    client.sendData("Hello, Server!");

    // Disconnect the client
    client.disconnect();

    // Stop the server (optional, depending on your design)
    server.disconnect();

    return 0;
}
