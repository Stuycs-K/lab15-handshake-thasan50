#include "pipe_networking.h"
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int from_client;
int to_client;

// Function to handle SIGINT (e.g., Ctrl+C on the server)
void handle_sigint(int signo) {
    printf("\nServer: Caught SIGINT, cleaning up and exiting...\n");
    remove("/tmp/mario"); // Remove WKP before exiting
    if (from_client) close(from_client);
    if (to_client) close(to_client);
    // exit(0);
}

int main() {
    signal(SIGINT, handle_sigint);  // Handle Ctrl+C
    signal(SIGPIPE, SIG_IGN);       // Ignore SIGPIPE to avoid crashing

    while (1) {
        printf("Server: Waiting for a client...\n");
        from_client = server_handshake(&to_client);
        printf("Server: Handshake completed with a client!\n");

        while (1) {
            int random_number = rand() % 101;
            printf("Server: Sending random number %d to client\n", random_number);

            if (write(to_client, &random_number, sizeof(random_number)) == -1) {
                perror("Server: Client disconnected or error writing.");
                break;
            }
            sleep(1);
        }

        // Proper cleanup
        close(from_client);
        close(to_client);
        printf("Server: Client disconnected. Ready to accept a new client.\n");
    }

    return 0;
}
