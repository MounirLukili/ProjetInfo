#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring> // for strlen
#include <arpa/inet.h> // for inet_ntoa
#include <cbor.h> // include CBOR library

int main() {
    // Create socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0); // Create a socket using IPv4 address family and TCP protocol
    if (clientSocket == -1) { // Check if socket creation was successful
        std::cerr << "Error creating socket\n"; // Print error message if socket creation failed
        return 1; // Return error code
    }

    // Server address and port
    struct sockaddr_in serverAddr; // Define a sockaddr_in struct to hold server address information
    serverAddr.sin_family = AF_INET; // Set address family to IPv4
    serverAddr.sin_port = htons(12345); // Server's port number (12345)
    serverAddr.sin_addr.s_addr = inet_addr("172.18.0.1"); // Example IP address

    // Connect to server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) { // Connect to the server
        std::cerr << "Error connecting to server\n"; // Print error message if connection fails
        close(clientSocket); // Close the client socket
        return 1; // Return error code
    }

    std::cout << "Connected to server. You can now send messages." << std::endl; // Print message indicating successful connection to the server

    while (true) {
        // Prompt user for message
        std::cout << "Enter message to send: "; // Prompt the user to enter a message
        std::string userInput; // String to hold user input
        std::getline(std::cin, userInput); // Read user input from the console

        // Create CBOR map with message
        cbor_item_t *message = cbor_new_definite_map(1); // Create a new CBOR map with one key-value pair
        struct cbor_pair msgPair; // Define a CBOR pair to hold the key-value pair
        msgPair.key = cbor_move(cbor_build_string("message")); // Set the key of the pair to "message"
        msgPair.value = cbor_move(cbor_build_string(userInput.c_str())); // Set the value of the pair to the user input
        cbor_map_add(message, msgPair); // Add the pair to the CBOR map

        // Serialize CBOR message
        unsigned char *buffer; // Pointer to hold the serialized CBOR message
        size_t buffer_size; // Variable to hold the size of the serialized message
        buffer_size = cbor_serialize_alloc(message, &buffer, &buffer_size); // Serialize the CBOR message

        // Send CBOR message to server
        ssize_t bytesSent = send(clientSocket, buffer, buffer_size, 0); // Send the serialized CBOR message to the server
        if (bytesSent == -1) { // Check if an error occurred while sending the message
            std::cerr << "Error sending message to server\n"; // Print error message if sending fails
            cbor_decref(&message); // Free memory allocated for the CBOR message
            free(buffer); // Free memory allocated for the buffer
            break; // Exit loop if error occurs
        } else if (bytesSent != buffer_size) { // Check if the entire message was not sent
            std::cerr << "Incomplete message sent to server\n"; // Print error message
            cbor_decref(&message); // Free memory allocated for the CBOR message
            free(buffer); // Free memory allocated for the buffer
            break; // Exit loop if error occurs
        }

        // Free memory
        cbor_decref(&message); // Free memory allocated for the CBOR message
        free(buffer); // Free memory allocated for the buffer

        // Wait for a while before sending the next message
        sleep(1); // Adjust the delay as needed
    }

    // Show message if client disconnects
    std::cout << "Client disconnected." << std::endl; // Print message indicating client disconnection

    // Close socket
    close(clientSocket); // Close the client socket

    return 0; // Return success code
}

