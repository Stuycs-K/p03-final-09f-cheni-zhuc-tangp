#include "networking.h"

#define NUMBER_OF_CLIENTS 100
struct client clients[NUMBER_OF_CLIENTS];

void server_logic(int fd, char * message, fd_set * master, int max_fd, int listen_socket) {
  char response[BUFFER_SIZE];

  char *line = message + 1; //skips over '/'
  char *token = strsep(&line, " ");
  char *args = line;

  if (message[0] == '/'){
    if (strcmp(token, "NAME") == 0) {
      struct client cli;
      cli.fd = fd
      strcpy(cli.name, message);
      clients[fd] = cli; 

      strncpy(clients[fd % NUMBER_OF_CLIENTS].name, line, 255);
      snprintf(response, sizeof(response), "Name %s logged", clients[fd % NUMBER_OF_CLIENTS].name);
    } 
    else if (strcmp(line, "WHO") == 0) {
      for (int i = 0; i <= max_fd; i++){
        if (FD_ISSET(i, master)){
          if (i != listen_socket && i != fd){
            strcat(response, clients[i].name);
          }
        }
      }
    } 
    else if (strcmp(process, "QUIT") == 0) {
      strncpy(response, "Quitting", sizeof(response));
      clients[fd] = NULL;
      //send(fd, response, sizeof(response), 0);
      // main loop handles close
    }
  } 
  else{
      snprintf(response, sizeof(response), "%s", line);
  } 
  
  //send back to other cliens 
  for (int i = 0; i <= max_fd; i++){
    if (FD_ISSET(i, master)){
      if (i != listen_socket && i != fd){
        err(send(i, response, strlen(response), 0), "In server logic");
      }
    }
  }
}

int main(int argc, char *argv[] ) {
  int listen_socket = server_setup();
  for(int i=0; i<NUMBER_OF_CLIENTS; i++) strcpy(names[i], "Unnamed");

  fd_set master, read_fds;
  FD_ZERO(&master);
  FD_SET(listen_socket, &master);
  int max_fd = listen_socket;

  printf("Server started\n");

  while(1){
    read_fds = master;
    if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1) {
      perror("select error");
      continue;
    }

    for (int fd = 0; fd <= max_fd; fd++) { //for all fds
      if (FD_ISSET(fd, &read_fds)) { //fd is in set
        if (fd == listen_socket) {
          int client_fd = server_tcp_handshake(listen_socket);
          if (client_fd != -1) {
            //FD_CLR(fd, &master); //remove listen socket and add client socket
            FD_SET(client_fd, &master); //add fd to master
            if (client_fd > max_fd) max_fd = client_fd;
            printf("client fd %d connected\n", client_fd);
          }
        }
        else {
          char buffer[BUFFER_SIZE];
          int recv_code = recv(fd, buffer, sizeof(buffer) - 1, 0);
          if (recv_code <= 0) {
            printf("Client disconnected: %s\n", names[fd % NUMBER_OF_CLIENTS]);
            close(fd);
            FD_CLR(fd, &master);
          }
          else {
            buffer[recv_code] = '\0';
            printf("%s sent: %s\n", names[fd % NUMBER_OF_CLIENTS], buffer);
            server_logic(fd, buffer, &master, max_fd, listen_socket);
          }
        }
      }
    }
  }
  return 0;
}
