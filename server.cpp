#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring> // for strlen
#include <arpa/inet.h> // for inet_ntoa
#include <cbor.h> // include CBOR library
#include <errno.h> // for errno

// Function to check if a socket is still connected
bool isSocketConnected(int socket) {
    struct sockaddr_in addr;
    socklen_t addrLen = sizeof(addr);
    return getpeername(socket, (struct sockaddr*)&addr, &addrLen) == 0;
}

int main() {
    // Create socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket\n";
        return 1;
    }

    // Bind socket to port
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_addr.s_addr = inet_addr("172.18.0.1");
    serverAddr.sin_port = htons(12345); // Port number
    
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding socket\n";
        return 1;
    }

    // Listen
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Error listening on socket\n";
        return 1;
    }

    std::cout << "Server started. Waiting for connections..." << std::endl;

    while (true) {
        // Accept incoming connections
        struct sockaddr_in clientAddr;
        socklen_t clientAddrSize = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrSize);
        if (clientSocket == -1) {
            std::cerr << "Error accepting connection\n";
            continue;
        }

        // Display client information
        char clientInfo[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(clientAddr.sin_addr), clientInfo, INET_ADDRSTRLEN);
        std::cout << "Client connected: " << clientInfo << ":" << ntohs(clientAddr.sin_port) << std::endl;

        while (true) {
            // Check if client socket is still connected
            if (!isSocketConnected(clientSocket)) {
                std::cout << "Lost connection with client." << std::endl;
                close(clientSocket);
                break;
            }

            // Receive CBOR message from client
            unsigned char buffer[1024];
            ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
            if (bytesReceived == -1) {
                if (errno == EWOULDBLOCK || errno == EAGAIN) {
                    // No data available, continue waiting
                    continue;
                }
                std::cerr << "Error receiving data from client\n";
                close(clientSocket);
                break;
            }

            if (bytesReceived == 0) {
                std::cout << "Client disconnected." << std::endl;
                close(clientSocket);
                break;
            }

            // Load CBOR message
            cbor_item_t *receivedMessage;
            struct cbor_load_result result;
            receivedMessage = cbor_load(buffer, bytesReceived, &result);
            if (receivedMessage == NULL) {
                std::cerr << "Error loading CBOR message\n";
                close(clientSocket);
                break;
            }

            // Extract message from CBOR map
            cbor_pair *messagePairs;
            size_t mapSize = cbor_map_size(receivedMessage);
            messagePairs = cbor_map_handle(receivedMessage);
            if (mapSize == 0) {
                std::cerr << "Error: Received message map is empty\n";
                cbor_decref(&receivedMessage);
                close(clientSocket);
                break;
            }

            // Get the first pair from the map
            cbor_item_t *messageValue = messagePairs[0].value;
            if (cbor_typeof(messageValue) != CBOR_TYPE_STRING) {
                std::cerr << "Error: Received message is not a string\n";
                cbor_decref(&receivedMessage);
                close(clientSocket);
                break;
            }

            // Print the received message
            unsigned char *messageData = cbor_string_handle(messageValue);
            size_t messageSize = cbor_string_length(messageValue);
            std::cout << "Received from client: " << std::string(messageData, messageData + messageSize) << std::endl;

            // Free memory
            cbor_decref(&receivedMessage);
        }
    }

    // Close server socket (this part of the code is never reached)
    close(serverSocket);

    return 0;
}

