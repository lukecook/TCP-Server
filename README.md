# TCP-Server

A lightweight TCP server implementation in C++ that demonstrates basic socket programming concepts.

## Overview

This project implements a simple TCP server that accepts client connections, receives client messages, and echoes them back to the client. This program was built to learn and explore fundamental networking concepts in C++.

## Features

- Single client connection support
- Message echo functionality
- Client disconnection handling
- Error handling for socket operations
- Simple command-line interface

## Compilation

Compile the server using: g++ -std=c++17 server.cpp -o server

## Usage

1. Start the server by specifying a port number: ./server <port#>
2. If you receive an error immediately, check if the port is already in use: lsof -i :<port#>
3. Once the server is running, open a new terminal and connect using netcat: nc localhost <port#>
4. Type messages and press Enter to send them. The server will echo back your messages.
5. To disconnect, press Ctrl+C in the client terminal. The server will display a disconnection message and close the socket.

## Credits
This program was heavily inspired by Benedikt Thordarson c++ tutorial on basic TCP server. Watch his video explaining this program to learn more!
