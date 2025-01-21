#include "pipe_networking.h"
//UPSTREAM = to the server / from the client
//DOWNSTREAM = to the client / from the server
/*=========================
  server_setup

  creates the WKP and opens it, waiting for a  connection.
  removes the WKP once a connection has been made

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_setup() {
  int from_client;
  char path[] = "/tmp/mario";
  int a = mkfifo(path, 0666);
  if (a == -1) {
    perror("Creating WKP failed");
    exit(1);
  }
  printf("Creating WKP succeeded");
  // Blocks server until client opens pipe for writing
  from_client = open(path, O_RDONLY);
  if (from_client == -1) {
    perror("Opening WKP failed");
    remove(path);
    exit(1);
  }
  printf("WKP opened");
  remove(path);
  //Returns file descriptor of pipe from client to server
  return from_client;
}

/*=========================
  server_handshake
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe (Client's private pipe).

  returns the file descriptor for the upstream pipe (see server setup).
  =========================*/
int server_handshake(int *to_client) {
  int from_client = server_setup();

  char client_pipe[256];
  if (read(from_client, client_pipe, sizeof(client_pipe)) <= 0) {
    perror("Reading client pipe name failed");
    close(from_client);
    exit(1);
  }
  printf("Received client pipe name: %s\n", client_pipe);

  *to_client = open(client_pipe, O_WRONLY);
  if (*to_client == -1) {
    perror("Opening client pipe failed");
    close(from_client);
    exit(1);
  }
  printf("Opened client pipe\n");

  char confirmation[] = "Handshake successful!";
  if (write(*to_client, confirmation, sizeof(confirmation)) == -1) {
    perror("Writing to client pipe failed");
    close(from_client);
    close(*to_client);
    exit(1);
  }
  printf("Writing to client pipe successful\n");

  char ack[256];
  if (read(from_client, ack, sizeof(ack)) <= 0) {
    perror("Reading acknowledgment failed");
    close(from_client);
    close(*to_client);
    exit(1);
  }
  printf("Received acknowledgment: %s\n", ack);
  //Waiting, and eventually the client will know it exists and read the name
  //Reads from the client, which has the other pipe
  //Creates an unnamed pipe
  return from_client;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  int from_server;
  char pid[100];
  sprintf(pid, "/tmp/%d", getpid());
  //Client to server pipe is WKP, and Server to client pipe is named the pid
  if (mkfifo(pid, 0666) == -1) {
    perror("Error making server to client pipe");
    exit(1);
  }
  *to_server = open("/tmp/mario", O_WRONLY);
  if (*to_server == -1) {
    perror("Opening WKP failed");
    remove(pid);
    exit(1);
  }
  if (write(*to_server, pid, sizeof(pid)) == -1) {
    perror("Writing private pipe name failed");
    close(*to_server);
    remove(pid);
    exit(1);
  }
  from_server = open(pid, O_RDONLY);
  if (from_server == -1) {
    perror("Opening private pipe failed");
    close(*to_server);
    remove(pid);
    exit(1);
  }
  char synack[256];
  if (read(from_server, synack, sizeof(synack)) <= 0) {
    perror("Reading server confirmation failed");
    close(*to_server);
    close(from_server);
    remove(pid);
    exit(1);
  }
  printf("Received server confirmation: %s\n", synack);

  char ack[] = "Acknowledgment received!";
  if (write(*to_server, ack, sizeof(ack)) == -1) {
    perror("Sending acknowledgment failed");
    close(*to_server);
    close(from_server);
    remove(pid);
    exit(1);
  }
  remove(pid);
  return from_server;
}


/*=========================
  server_connect
  args: int from_client

  handles the subserver portion of the 3 way handshake

  returns the file descriptor for the downstream pipe.
  =========================*/
int server_connect(int from_client) {
  int to_client  = 0;
  char client_pipe[256];
  if (read(from_client, client_pipe, sizeof(client_pipe)) <= 0) {
    perror("Reading client pipe name failed");
    close(from_client);
    exit(1);
  }
  to_client = open(client_pipe, O_WRONLY);
  if (to_client == -1) {
    perror("Opening client pipe failed");
    close(from_client);
    exit(1);
  }

  return to_client;
}
