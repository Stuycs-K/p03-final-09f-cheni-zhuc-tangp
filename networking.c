#include "networking.h"

/*
Create and bind a socket.
* Place the socket in a listening state.
* returns the socket descriptor
*/
int server_setup() {
  //setup structs for getaddrinfo
  struct addrinfo * hints, * results;//results is allocated in getaddrinfo
  hints = calloc(1,sizeof(struct addrinfo));

  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM; //TCP socket
  hints->ai_flags = AI_PASSIVE; //only needed on server
  //getaddrinfo("127.0.0.1", PORT, hints, &results);
  int status = getaddrinfo(NULL, PORT, hints, &results);  //Server sets node to NULLsults?
  if (status == -1) err(status, "In server setup - getaddrinfo");

  //create the socket
  int clientd;//store the socket descriptor here
  clientd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
  if (clientd == -1) err(clientd, "In server setup - socket");

  //this code should get around the address in use error
  int yes = 1;
  int sockOpt =  setsockopt(clientd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
  err(sockOpt,"sockopt  error");

  //bind the socket to address and port
  int bind_errno = bind(clientd, results->ai_addr, results->ai_addrlen);
  err(bind_errno, "In server steup - bind");

  //set socket to listen state
  err(listen(clientd, 10), "In Server setup - listen");

  
  
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

  //select
  socklen_t sock_size;
  struct sockaddr_storage client_address;
  sock_size = sizeof(client_address);
  fd_set read_fds;
  char buff[1025]="";

  FD_ZERO(&read_fds);
  FD_SET(STDIN_FILENO, &read_fds);
  FD_SET(listen_socket,&read_fds);
  int i = select(listen_socket+1, &read_fds, NULL, NULL, NULL);
  //if standard in, use fgets
  if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      fgets(buff, sizeof(buff), stdin);
      buff[strlen(buff)-1]=0;
      printf("Recieved from terminal: '%s'\n",buff);
  }
  // if socket
  if (FD_ISSET(listen_socket, &read_fds)) {
      //accept the connection
      int client_socket = accept(listen_socket,(struct sockaddr *)&client_address, &sock_size);
      if (client_socket == -1) err(client_socket, "In Server tcp handshake"); 
      printf("Connected, waiting for data.\n");
  }

  return client_socket;
}


/*Connect to the server
 *return the to_server socket descriptor
 *blocks until connection is made.*/
int client_tcp_handshake(char * server_address) {

  //getaddrinfo
  struct addrinfo * hints, * results;//results is allocated in getaddrinfo
  hints = calloc(1,sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM; //TCP socket
  hints->ai_flags = 0;
  getaddrinfo(server_address, PORT, hints, &results);  //Server sets node to NULL

  int serverd;//store the socket descriptor here
  //create the socket
  serverd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
  err(serverd, "In client_tcp_handshake");

  //connect() to the server
  err(connect(serverd, results->ai_addr, results->ai_addrlen), "In Connect");

  free(hints);
  freeaddrinfo(results);

  return serverd;
}

void err(int i, char*message){
  if(i < 0){
    printf("Error: %s - %s\n",message, strerror(errno));
    exit(1);
  }
}