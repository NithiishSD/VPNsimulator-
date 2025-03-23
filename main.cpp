#include <iostream>
#include <string>
#include "vpnclasses.h"

using namespace std;

int main() {
    VPNClient client("127.0.0.1", 8080);
    VPNServer server("127.0.0.1", 8080);

    client.connect();
    client.sendData("Hello, Server!");
    client.disconnect();

    server.connect();
    server.acceptConnection();
    server.disconnect();

    return 0;
}
