#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sys/socket.h> // socket functions
#include <netinet/in.h> // sockaddr_in structure
#include <arpa/inet.h> // inet_ntoa function

using std::cout, std::cerr, std::endl, std::string, std::atoi, std::to_string;

const int BUFFER {1024};

int main(int argc, char ** argv)
{
    if(argc < 2)
    {
        cout << "Usage: " << argv[0] << " <port number>" << endl;
        return EXIT_SUCCESS;
    }

    int port = atoi(argv[1]);

    // Socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        cerr << "Could not create socket" << endl;
        return EXIT_FAILURE;
    }

    // Bind
    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));

    address.sin_family = AF_INET;
    address.sin_port = htons(port); // htons: host to network short (flips bytes if little endian sys)

    int bind_value = bind(sock, (struct sockaddr *) &address, sizeof(address));
    if(bind_value < 0)
    {
        cerr << "Could not bind: check port using lsof -i <port#>" << endl;
        return EXIT_FAILURE;
    }

    // Listen
    int listen_value = listen(sock, 1);
    if(listen_value < 0)
    {
        cerr << "Could not listen" << endl;
        return EXIT_FAILURE;
    }

    // Accept
    struct sockaddr_in remote_address;
    memset(&remote_address, 0, sizeof(remote_address));
    socklen_t remote_addr_len = sizeof(remote_address); // socklen_t: unsigned int

    cout << "Waiting for a new connection. New tab run nc localhost <port#> to connect" << endl;
    int client_socket = accept(sock, (struct sockaddr *) &remote_address, &remote_addr_len);
    if(client_socket < 0)
    {
        cerr << "Could not accept" << endl;
        return EXIT_FAILURE;
    }
    
    string client_ip = inet_ntoa(remote_address.sin_addr);
    int remote_port = ntohs(remote_address.sin_port);

    cout << "Accepted new client @ " << client_ip << ":" << remote_port << endl;

    char buffer[BUFFER];

    bool flag {true};
    while(flag)
    {
        memset(buffer, 0, BUFFER);

        // Recieve
        int bytes_recieved = recv(client_socket, buffer, BUFFER - 1, 0);
        if(bytes_recieved < 0)
        {
            cerr << "Could not recieve" << endl;
            return EXIT_FAILURE;
        }
        else if(bytes_recieved == 0)
        {
            cout << "Client at " << client_ip << ":" << remote_port << " has disconnected." << endl;
            flag = false;
        }
        if(buffer[bytes_recieved - 1] == '\n')
            buffer[bytes_recieved - 1] = 0;

        if(bytes_recieved != 0)
            cout << "Client message: \"" << buffer << "\"" << endl;

        // Send
        string response = "Hello client at " + client_ip + ":" + to_string(remote_port) +
            ". Your message was \n\"" + string(buffer) + "\"\n";

        int bytes_sent = send(client_socket, response.c_str(), response.length(), 0); // c_str() converts to cstring
        if(bytes_sent < 0)
        {
            cerr << "Could not send" << endl;
            return EXIT_FAILURE;
        }
    }

    cout << "Shutting down socket." << endl;
    shutdown(client_socket, SHUT_RDWR);
}
