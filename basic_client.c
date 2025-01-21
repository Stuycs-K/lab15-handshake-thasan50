#include "pipe_networking.h"
#include <signal.h>

int to_server;
int from_server;

// Handle `SIGINT` to exit gracefully
void handle_sigint(int signo) {
    printf("\nClient: Exiting\n");
    close(to_server);
    close(from_server);
    exit(0);
}

int main() {
    signal(SIGINT, handle_sigint); // Register signal handler

    printf("Client: Attempting to connect to server\n");
    from_server = client_handshake(&to_server);
    printf("Client: Handshake completed!\n");

    while (1) {
        int received_number;
        if (read(from_server, &received_number, sizeof(received_number)) > 0) {
            printf("Client: Received random number: %d\n", received_number);
        } else {
            perror("Client: Error reading from server");
            break;
        }
    }

    return 0;
}
