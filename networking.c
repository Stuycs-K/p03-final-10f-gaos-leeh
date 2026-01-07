#include "networking.h"

void err(int i, char *message) {
    if (i < 0) {
        printf("Error: %s - %s\n", message, strerror(errno));
        exit(1);
    }
}

int server_setup() {
  //setup structs for getaddrinfo
  struct addrinfo* hints = (struct addrinfo*) malloc(sizeof(struct addrinfo));
  struct addrinfo* results = (struct addrinfo*) malloc(sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM;
  hints->ai_flags = AI_PASSIVE;
  err(getaddrinfo(NULL, PORT, hints, &results), "");

  //create the socket
  int clientd = socket(AF_INET, SOCK_STREAM, 0); err(clientd, "socket creation fail"); //store the socket descriptor here

  //this code should get around the address in use error
  int yes = 1;
  int sockOpt =  setsockopt(clientd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
  err(sockOpt,"sockopt  error");

  //bind the socket to address and port
  err(bind(clientd, results->ai_addr, results->ai_addrlen), "bind error");
  printf("bind complete\n");

  //set socket to listen state
  printf("server listening...\n");
  err(listen(clientd, BACKLOG), "listen error");

  //free the structs used by getaddrinfo
  free(hints);
  freeaddrinfo(results);

  return clientd;
}

/*Accept a connection from a client
 *return the socket descriptor for the new socket connected to the client
 *blocks until connection is made.
 */
int server_tcp_handshake(int listen_socket){
    int client_socket;

    //accept() the client connection
    struct sockaddr* client_id = (struct sockaddr*) malloc(sizeof(struct sockaddr));
    int addrlen = sizeof(client_id);

    client_socket = accept(listen_socket, (struct sockaddr *)&client_id, &addrlen); err(client_socket, "");
    printf("server connected\n");

    return client_socket;
}


/*Connect to the server
 *return the to_server socket descriptor
 *blocks until connection is made.*/
int client_tcp_handshake(char * server_address) {
  //getaddrinfo
  struct addrinfo* hints = (struct addrinfo*) malloc(sizeof(struct addrinfo));
  struct addrinfo* results = (struct addrinfo*) malloc(sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM;
  hints->ai_flags = AI_PASSIVE;
  err(getaddrinfo(server_address, PORT, hints, &results), "");

  //store the socket descriptor here
  int serverd = socket(results->ai_family, results->ai_socktype, results->ai_protocol); err(serverd, "");
  //create the socket
  //connect() to the server
  connect(serverd, results->ai_addr, results->ai_addrlen);

  free(hints);
  freeaddrinfo(results);

  return serverd;
}
