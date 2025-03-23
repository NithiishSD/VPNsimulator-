#include <iostream>
#include <string>
#include <openssl/thread.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "vpnclasses.h"

using namespace std;


Logger* Logger::instance = nullptr;
VPNNode::VPNNode(string ip, int port) {
    ipAddress = ip;
    Port = port;
}
void VPNServer::connect() {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        Logger::getInstance().log("WSAStartup failed.");
        return;
    }

    // Create a socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        Logger::getInstance().log("Error: Failed to create socket. Error code: " + std::to_string(WSAGetLastError()));
        WSACleanup();
        return;
    }

    // Set up the server address structure
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(Port); // Use the port variable from the class
    if (inet_pton(AF_INET, ipAddress.c_str(), &serverAddress.sin_addr) <= 0) {
        Logger::getInstance().log("Error: Invalid address or address not supported.");
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    // Bind the socket to the server address
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        Logger::getInstance().log("Error: Binding failed! Error code: " + std::to_string(WSAGetLastError()));
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) < 0) {
        Logger::getInstance().log("Error: Listening failed! Error code: " + to_string(WSAGetLastError()));
        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    Logger::getInstance().log("VPN server started at " + ipAddress + ":" + to_string(Port));

    // Accept incoming connections in a loop
    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            Logger::getInstance().log("Error: Failed to accept connection. Error code: " + to_string(WSAGetLastError()));
            continue;
        }

        // Handle the client connection in a separate thread
        std::thread clientThread(handleClient, clientSocket);
        clientThread.detach();
    }

    // Clean up (this code is unreachable in the current implementation)
    closesocket(serverSocket);
    WSACleanup();
}
VPNClient::VPNClient(string ip, int Port) : VPNNode(ip, Port) {
    encryption=new EncryptionAlgorithm(); // Corrected memory allocation
}
void VPNClient::connect() {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        Logger::getInstance().log("WSAStartup failed.");
        return;
    }

    // Create a socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        Logger::getInstance().log("Error: Failed to create socket. Error code: " + std::to_string(WSAGetLastError()));
        WSACleanup();
        return;
    }

    // Set up the server address structure
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(Port); // Use the port variable from the class
    if (inet_pton(AF_INET, ipAddress.c_str(), &serverAddress.sin_addr) <= 0) {
        Logger::getInstance().log("Error: Invalid address or address not supported.");
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    // Connect to the server
    if (::connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        Logger::getInstance().log("Error: Connection failed! Error code: " + std::to_string(WSAGetLastError()));
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    Logger::getInstance().log("Connected to VPN server at " + ipAddress + ":" + std::to_string(Port));

    // Close the socket and clean up Winsock
    closesocket(clientSocket);
    WSACleanup();
}
void VPNClient::sendData(string data) {
    cout << "Sending data: " << data << endl;
}
void VPNClient::disconnect() {
    cout << "Disconnecting from VPN server." << endl;
}


void handleClient(SOCKET clientSocket) {
    char buffer[1024] = {0};
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0); // Use recv instead of fread
    if (bytesRead > 0) {
        Logger::getInstance().log("Received data: " + string(buffer, bytesRead));
    }
    closesocket(clientSocket); // Corrected socket closure
}
void VPNServer::acceptConnection() {
    cout << "Accepting client connection..." << endl;
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
