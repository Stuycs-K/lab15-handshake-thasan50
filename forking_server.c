#include "pipe_networking.h"
#include <signal.h>

// Function to handle SIGINT (cleanup before exiting)
void handle_sigint(int signo) {
    printf("\nServer: Caught SIGINT, cleaning up and exiting\n");
    remove("/tmp/mario"); // Remove WKP
    exit(0);
}

int server_handshake_half(int *to_client, int from_client) {
    char buffer[HANDSHAKE_BUFFER_SIZE];

    // Read client's message
    if (read(from_client, buffer, sizeof(buffer)) <= 0) {
        perror("Subserver: Error reading from client");
        return -1;
    }
    printf("Subserver: Received client message: %s\n", buffer);

    // Send acknowledgement to client
    *to_client = open(buffer, O_WRONLY);
    if (*to_client == -1) {
        perror("Subserver: Error opening private FIFO");
        return -1;
    }
    if (write(*to_client, ACK, sizeof(ACK)) == -1) {
        perror("Subserver: Error writing to client");
        return -1;
    }

    // Read final acknowledgement from client
    if (read(from_client, buffer, sizeof(buffer)) <= 0) {
        perror("Subserver: Error reading final ACK from client");
        return -1;
    }
    printf("Subserver: Handshake with client complete.\n");

    return 0;
}

int server_setup();
int server_handshake_half(int *to_client, int from_client);

int main() {
    signal(SIGINT, handle_sigint); // Register SIGINT handler
    signal(SIGPIPE, SIG_IGN);      // Ignore SIGPIPE to prevent broken pipe nonsense

    while (1) {
        printf("Server: Waiting for a client\n");
        int from_client = server_setup();
        printf("Server: Connection received, forking subserver\n");

        // Fork a subserver to handle the client
        int f = fork();
        if (f == 0) {
            // Child process (subserver)
            int to_client;
            server_handshake_half(&to_client, from_client);

            printf("Subserver: Handshake completed! Starting communication\n");
            while (1) {
                int random_number = rand() % 101;
                printf("Subserver: Sending random number %d to client\n", random_number);

                if (write(to_client, &random_number, sizeof(random_number)) == -1) {
                    perror("Subserver: Client disconnected or error writing.");
                    break;
                }
                sleep(1);
            }

            // Cleanup in subserver
            close(from_client);
            close(to_client);
            printf("Subserver: Client disconnected. Exiting.\n");
            exit(0);
        } else {
            // Parent process
            close(from_client); // Parent doesn't need the client's file descriptor
        }
    }

    return 0;
}
