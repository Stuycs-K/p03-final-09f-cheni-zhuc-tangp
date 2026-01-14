#include "networking.h"

#define NUMBER_OF_CLIENTS 100
char names[NUMBER_OF_CLIENTS][256];

void server_logic(int fd, char * message, fd_set * master, int max_fd, int listen_socket) {
  char response[BUFFER_SIZE];

  if (strncmp(message, "/", 1) == 0) {
    if (strncasecmp(message, "/NAME ", 6) == 0) {
      strncpy(names[fd % NUMBER_OF_CLIENTS], message + 6, 255);
      snprintf(response, sizeof(response), "Renamed to: %s", names[fd % NUMBER_OF_CLIENTS]);
      send(fd, response, strlen(response), 0);
      return;
    } else if (strncasecmp(message, "/WHO", 4) == 0) {
      strcpy(response, "Active users: ");
      for(int i = 0; i <= max_fd; i++) {
        if(FD_ISSET(i, master) && i != listen_socket) {
          strncat(response, "\n", 2);
          strncat(response, names[i % NUMBER_OF_CLIENTS], sizeof(response) - strlen(response) - 1);
          //strncat(response, " ", sizeof(response) - strlen(response) - 1);
        }
      }
      send(fd, response, strlen(response), 0);
      return;
    } else if (strncasecmp(message, "/QUIT", 5) == 0) {
      strcpy(response, "Quitting...");
      send(fd, response, strlen(response), 0);
      close(fd);
      FD_CLR(fd, master);
      return;
    } else {
      snprintf(response, sizeof(response), "Unknown command: %s", message);
      send(fd, response, strlen(response), 0);
      return;
    }
  } else{
    snprintf(response, sizeof(response), "[%s]: %s\n", names[fd % NUMBER_OF_CLIENTS], message);
    send(fd, response, strlen(response), 0);
  }

  //loop through all fd here
  for (int i = 0; i <= max_fd; i++) {
    if (FD_ISSET(i, master)) {
      if (i != listen_socket && i != fd) {
        send(i, response, strlen(response), 0);
      }
    }
  }
}


int main(int argc, char *argv[] ) {
 // struct client clients[NUMBER_OF_CLIENTS];

  int listen_socket = server_setup();
  //for(int i=0; i<NUMBER_OF_CLIENTS; i++) strcpy(clients[i].name, "Unnamed");

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
            //printf("Client disconnected: %s\n", clients[fd % NUMBER_OF_CLIENTS].name);
            printf("Client disconnected: %s\n", names[fd % NUMBER_OF_CLIENTS]);
            close(fd);
            FD_CLR(fd, &master);
          }
          else {
            buffer[recv_code] = '\0';
            printf("%s sent: %s\n", names[fd % NUMBER_OF_CLIENTS], buffer);
            //server_logic(fd, buffer, &master, max_fd, listen_socket, clients);
            server_logic(fd, buffer, &master, max_fd, listen_socket);
          }
        }
      }
    }
  }
  return 0;
}
