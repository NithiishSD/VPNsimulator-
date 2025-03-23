#include <iostream>
#include <string>
#include <openssl/thread.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "vpnclasses.h"

using namespace std;

VPNNode::VPNNode() {
    ipAddress = "111.222.333.444";
    Port = 4;
}

VPNNode::VPNNode(string ip, int port) {
    ipAddress = ip;
    Port = port;
}

VPNClient::VPNClient(string ip, int port) : VPNNode(ip, port) {
    encryption=new EncryptionAlgorithm(); // Corrected memory allocation
}

void VPNClient::connect() {
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        cout << "Error: Failed to create socket." << endl;
        return;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(Port);
    inet_pton(AF_INET, ipAddress.c_str(), &serverAddress.sin_addr);

    if (::connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        cout << "Error: Connection failed!" << endl;
        closesocket(clientSocket); // Corrected socket closure
        return;
    }
    cout << "Connected to VPN server at " << ipAddress << ":" << Port << endl;
    closesocket(clientSocket); // Corrected socket closure
}

void handleClient(SOCKET clientSocket) {
    char buffer[1024] = {0};
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0); // Use recv instead of fread
    if (bytesRead > 0) {
        Logger::getInstance().log("Received data: " + string(buffer, bytesRead));
    }
    closesocket(clientSocket); // Corrected socket closure
}

void VPNServer::connect() {
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        Logger::getInstance().log("Error: Failed to create socket.");
        return;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(Port);
    inet_pton(AF_INET, ipAddress.c_str(), &serverAddress.sin_addr);

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        Logger::getInstance().log("Error: Binding failed!");
        closesocket(serverSocket); // Corrected socket closure
        return;
    }
    listen(serverSocket, 5);
    Logger::getInstance().log("VPN server started at " + ipAddress + ":" + to_string(Port));

    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            Logger::getInstance().log("Error: Failed to accept connection.");
            continue;
        }
        thread clientThread(handleClient, clientSocket);
        clientThread.detach();
    }
}

void VPNServer::disconnect() {
    Logger::getInstance().log("VPN server stopped.");
}

string AESEncryption::encrypt(string data) {
    // Simulate AES encryption
    return "AES_Encrypted_" + data;
}

string AESEncryption::decrypt(string data) {
    // Simulate AES decryption
    return data.substr(14);
}
