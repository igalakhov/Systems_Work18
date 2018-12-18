#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client
  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.
  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  // make a well known pipe
  printf("\nServer: creating a well known pipe\n");
  remove(WKP_NAME);
  int wkp = mkfifo(WKP_NAME, 0666);

  if(wkp == -1){
    printf("Server: something went wrong! Exiting...\n");
    exit(0);
  }
  printf("Server: well known pipe created! Opening on read end and waiting for client to connect\n");

  int downstream_pipe = open(WKP_NAME, O_RDONLY);

  printf("Server: client connected! removing well known pipe\n");
  remove(WKP_NAME);
  printf("Server: well known pipe removed!\n");

  printf("Server: waiting for private pipe name from client\n");

  int pp;
  char pp_str[15];
  read(downstream_pipe, &pp, 4);
  snprintf(pp_str, 10,"%d", pp);

  printf("Server: private pipe name recieved!\n");
  printf("Server: connecting to private pipe\n");

  int upstream_pipe = open(pp_str, O_WRONLY);

  printf("Server: connected! writing acknowledgement.\n");

  write(upstream_pipe, ACK, strlen(ACK) + 1);

  printf("Server: written! waiting for client acknowledgement\n");

  char client_message[HANDSHAKE_BUFFER_SIZE];

  read(upstream_pipe, client_message, HANDSHAKE_BUFFER_SIZE);

  printf("Server: recieved with message %s\n", client_message);
  printf("Server: three way handshake is done!\n");

  return 0;
}


/*=========================
  client_handshake
  args: int * to_server
  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.
  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  printf("Client: Creating private pipe\n");

  char pid_str[15];
  int pid = getpid();
  snprintf(pid_str, 10,"%d",(int)getpid());

  remove(pid_str);
  int pp = mkfifo(pid_str, 0666);

  if(pp == -1){
    printf("Client: Something went wrong! Exiting...\n");
    exit(0);
  }

  printf("Client: Connecting to server's well known pipe\n");

  int downstream_pipe = open(WKP_NAME, O_WRONLY);

  printf("Client: Connected! Sending private pipe name\n");

  write(downstream_pipe, &pid, sizeof(int));

  printf("Client: Sent! Waiting for server connection to private pipe\n");

  int upstream_pipe = open(pid_str, O_RDONLY);

  char server_message[HANDSHAKE_BUFFER_SIZE];

  read(upstream_pipe, server_message, HANDSHAKE_BUFFER_SIZE);

  printf("Client: Server connected with message %s!\n", server_message);
  printf("Client: Removing private pipe\n");
  remove(pid_str);
  printf("Client: Private Pipe Removed!\n");

  printf("Client: Writing acknowledgement to server\n");

  write(upstream_pipe, ACK, strlen(ACK) + 1);

  printf("Client: Written! Three way handshake is done!\n");

  // set values

  return 0;
}
