#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <vector>
#include <mutex>
#include <algorithm>

using namespace std;

vector<int> clients;
mutex mtx;

void broadcastMessage(string message, int senderSocket) {
    lock_guard<mutex> lock(mtx);
    for(int clientSocket : clients) {
        if(clientSocket != senderSocket) {
            send(clientSocket, message.c_str(), message.size(), 0);
        }
    }
}

void handleClient(int clientSocket) {
    char buffer[1024];

    while(true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

        if(bytesReceived <= 0) {
            cout << "Client disconnected: " << clientSocket << endl;
            break;
        }

        string msg = "Client[" + to_string(clientSocket) + "]: " + string(buffer);
        cout << msg << endl;

        broadcastMessage(msg, clientSocket);
    }

    // Remove client from list
    {
        lock_guard<mutex> lock(mtx);
        clients.erase(std::remove(clients.begin(), clients.end(), clientSocket), clients.end());
    }

    close(clientSocket);
}

int main() {
    int serverSocket;
    sockaddr_in serverAddr, clientAddr;
    socklen_t clientLen = sizeof(clientAddr);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket < 0) {
        cout << "Socket creation failed!" << endl;
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if(bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        cout << "Bind failed!" << endl;
        return 1;
    }

    if(listen(serverSocket, 5) < 0) {
        cout << "Listen failed!" << endl;
        return 1;
    }

    cout << "Server started on port 8080...\n";

    while(true) {
        int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientLen);

        if(clientSocket < 0) {
            cout << "Client accept failed!" << endl;
            continue;
        }

        cout << "New client connected: " << clientSocket << endl;

        {
            lock_guard<mutex> lock(mtx);
            clients.push_back(clientSocket);
        }

        thread t(handleClient, clientSocket);
        t.detach();
    }

    close(serverSocket);
    return 0;
}
