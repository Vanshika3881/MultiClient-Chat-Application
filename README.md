# Multi-Client Chat Application (C++ | Linux | TCP/IP)

A multi-client client-server chat application implemented using TCP socket programming in C++ on Linux.

## Features
- Supports multiple clients simultaneously
- Uses multithreading for handling concurrent client connections
- Broadcasts messages to all connected clients
- Handles client disconnect gracefully

## Technologies Used
- C++
- Linux Socket Programming
- TCP/IP
- Multithreading (pthread / std::thread)

## How to Compile
```bash
g++ server.cpp -o server -pthread
g++ client.cpp -o client -pthread



