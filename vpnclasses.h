#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <fstream>
#pragma comment(lib, "ws2_32.lib")
#ifndef vpnclasses.h
#define vpnclasses.h

using namespace std;
class EncryptionAlgorithm
{

    public:
        string encrypt(string);
        string decrypt(string);


};
class VPNNode //to create a vpn node for serverr and client side
{
    protected:
        string ipAddress;
        int Port;
    public:
        VPNNode();
        VPNNode(string,int);
        virtual void connect();
        virtual void disconnect();

};
class VPNClient:public VPNNode
{
    private:
        EncryptionAlgorithm* encryption;

    public:
        VPNClient(string ip,int port);
        void connect()override;
        void disconnect()override;
        void sendData(string);
};
class VPNServer:public VPNNode
{
    public:
        VPNServer(string ip,int port):VPNNode(ip,port){}
        void connect()override;
        void disconnect()override;
        void acceptConnection();

};


class AESEncryption {
public:
    string encrypt(string data);
    string decrypt(string data);
};
class Logger {
    private:
        std::ofstream logFile;
        Logger(){logFile.open("vpnlog.txt", std::ios::app);}
    public:
        static Logger& getInstance() {
        static Logger instance;
        return instance;
    }
    void log(const string& message) {
        time_t now = time(nullptr);
        logFile << ctime(&now) << " - " << message <<endl;
    }
};

#endif

