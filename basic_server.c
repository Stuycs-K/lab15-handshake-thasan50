#include "pipe_networking.h"

int main() {
    int to_client;
    int from_client;

    printf("Server: Waiting for client handshake...\n");
    from_client = server_handshake(&to_client);

    // Test the handshake by receiving and responding with a byte
    char received_byte;
    if (read(from_client, &received_byte, sizeof(received_byte)) == -1) {
        perror("Server: Failed to read from client");
        exit(1);
    }
    printf("Server: Received test byte '%c' from client\n", received_byte);

    char response_byte = 'S';  // Example byte from server
    if (write(to_client, &response_byte, sizeof(response_byte)) == -1) {
        perror("Server: Failed to write to client");
        exit(1);
    }
    printf("Server: Sent test byte '%c' to client\n", response_byte);

    printf("Server: Handshake and test completed successfully\n");

    return 0;
}

