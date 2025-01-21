#include "pipe_networking.h"

int main() {
    int to_server;
    int from_server;

    printf("Client: Starting handshake...\n");
    from_server = client_handshake(&to_server);

    // Test the handshake by sending and receiving a byte
    char test_byte = 'C';  // Example byte from client
    if (write(to_server, &test_byte, sizeof(test_byte)) == -1) {
        perror("Client: Failed to write to server");
        exit(1);
    }
    printf("Client: Sent test byte '%c' to server\n", test_byte);

    char response_byte;
    if (read(from_server, &response_byte, sizeof(response_byte)) == -1) {
        perror("Client: Failed to read from server");
        exit(1);
    }
    printf("Client: Received test byte '%c' from server\n", response_byte);

    printf("Client: Handshake and test completed successfully\n");

    return 0;
}
