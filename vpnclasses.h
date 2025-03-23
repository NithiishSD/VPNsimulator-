#ifndef vpnclasses_h
#define vpnclasses_h

#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <fstream>


using namespace std;

class Logger {
private:
    static Logger* instance;
    Logger() {} // Private constructor

public:
    static Logger& getInstance() {
        if (!instance) {
            instance = new Logger();
        }
        return *instance;
    }

    void log(const std::string& message) {
        std::cout << message << std::endl;
    }
};


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
        VPNNode(string,int);
        virtual void connect()=0;
        virtual void disconnect()=0;
        virtual ~VPNNode(){}

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

void handleClient(SOCKET);
#endif

