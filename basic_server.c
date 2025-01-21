#include "pipe_networking.h"
#include <signal.h>

int to_client;
int from_client;

// Handle `SIGINT` to exit gracefully
void handle_sigint(int signo) {
    printf("\nServer: Exiting\n");
    close(to_client);
    close(from_client);
    exit(0);
}

int main() {
    signal(SIGINT, handle_sigint); // Register signal handler
    srand(time(NULL)); // Seed random number generator

    printf("Server: Waiting for a client\n");
    from_client = server_handshake(&to_client);
    printf("Server: Handshake completed!\n");

    while (1) {
        int random_number = rand() % 101; // Generate a random number between 0 and 100
        printf("Server: Sending random number %d to client\n", random_number);

        if (write(to_client, &random_number, sizeof(random_number)) == -1) {
            perror("Server: Error sending random number");
            break;
        }

        sleep(1); // Wait for 1 second
    }

    return 0;
}


