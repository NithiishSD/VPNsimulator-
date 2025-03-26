#ifndef vpnclasses_h
#define vpnclasses_h

#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <fstream>
#include <vector>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <mapi.h>
#include <limits.h>
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
        virtual string encrypt(string)=0;
        virtual string decrypt(string)=0;
        virtual ~EncryptionAlgorithm() {}

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
    private:
     vector<std::thread> clientThreads;
     std::mutex mtx;
     int clientCounter = 0;
    void handleClient(int clientId);
    public:
        VPNServer(string ip,int port):VPNNode(ip,port){}
        void connect()override;
        void disconnect()override;
        void acceptConnection();

};


class AESEncryption : public EncryptionAlgorithm {
private:
    vector<unsigned char> key;

public:
    AESEncryption() {
        key.resize(AES_BLOCK_SIZE);
        RAND_bytes(key.data(), AES_BLOCK_SIZE);
    }

    string encrypt(const std::string& data) override {
        AES_KEY aesKey;
        AES_set_encrypt_key(key.data(), 128, &aesKey);

        std::vector<unsigned char> encrypted(data.size() + AES_BLOCK_SIZE);
        int len = data.size();
        AES_encrypt(reinterpret_cast<const unsigned char*>(data.data()), encrypted.data(), &aesKey);
        return std::string(encrypted.begin(), encrypted.end());
    }

    string decrypt(const string& data) override {
        AES_KEY aesKey;
        AES_set_decrypt_key(key.data(), 128, &aesKey);

        vector<unsigned char> decrypted(data.size());
        AES_decrypt(reinterpret_cast<const unsigned char*>(data.data()), decrypted.data(), &aesKey);
        return string(decrypted.begin(), decrypted.end());
    }
};

class Router {
private:
    std::map<std::string, std::map<std::string, int>> graph; // {source: {destination: cost}}
    std::map<std::string, std::string> routingTable; // {destination: next_hop}

    void bellmanFord(const std::string& source);

public:
    void addEdge(const std::string& source, const std::string& destination, int cost);
    void buildRoutingTable(const std::string& source);
    void printRoutingTable();
    std::string getNextHop(const std::string& destination);
};

void handleClient(SOCKET);
#endif

