#include "pipe_networking.h"

int main() {
  int to_client;
  int from_client;
  server_setup();
  from_client = server_handshake( &to_client );
  
}
