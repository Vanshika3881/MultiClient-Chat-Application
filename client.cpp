#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>

using namespace std;

void receiveMessages(int clientSocket) {
    char buffer[1024];

    while(true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

        if(bytesReceived <= 0) {
            cout << "\nDisconnected from server.\n";
            break;
        }

        cout << "\n" << buffer << endl;
        cout << "You: ";
        cout.flush();
    }
}

int main() {
    int clientSocket;
    sockaddr_in serverAddr;

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket < 0) {
        cout << "Socket creation failed!" << endl;
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);

    cout << "Enter Server IP (example 127.0.0.1): ";
    string ip;
    cin >> ip;

    if(inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr) <= 0) {
        cout << "Invalid IP address!" << endl;
        return 1;
    }

    if(connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        cout << "Connection failed!" << endl;
        return 1;
    }

    cout << "Connected to server!\n";

    thread t(receiveMessages, clientSocket);
    t.detach();

    cin.ignore();
    string message;

    while(true) {
        cout << "You: ";
        getline(cin, message);

        if(message == "exit") {
            cout << "Exiting...\n";
            break;
        }

        send(clientSocket, message.c_str(), message.size(), 0);
    }

    close(clientSocket);
    return 0;
}
