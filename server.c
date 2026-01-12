#include "networking.h"

#define NUMBER_OF_CLIENTS 100
char names[NUMBER_OF_CLIENTS][256];

void server_logic(int fd, char * message) {
  char response[BUFFER_SIZE];
    
  if (strncmp(message, "NAME ", 5) == 0) {
    strncpy(names[fd % NUMBER_OF_CLIENTS], message + 5, 255);
    snprintf(response, sizeof(response), "Name: %s", names[fd % NUMBER_OF_CLIENTS]);
  } else if (strncmp(message, "MSG ", 4) == 0) {
    snprintf(response, sizeof(response), "%s", message + 4);
  } else if (strcmp(message, "WHO") == 0) {
    strncpy(response, "uhhh idk man this hard", sizeof(response));
  } else if (strcmp(message, "QUIT") == 0) {
    strncpy(response, "Quitting", sizeof(response));
    send(fd, response, sizeof(response), 0);
    return; // main loop handles close
  } else snprintf(response, sizeof(response), "Unknown command: %s", message);


  send(fd, response, sizeof(response), 0);
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

    for (int fd = 0; fd <= max_fd; fd++) {
      if (FD_ISSET(fd, &read_fds)) {
        if (fd == listen_socket) {
          int client_fd = server_tcp_handshake(listen_socket);
          if (client_fd != -1) {
            FD_SET(client_fd, &master);
            if (client_fd > max_fd) max_fd = client_fd;
          }
        } else {
          char buffer[BUFFER_SIZE];
          int recv_code = recv(fd, buffer, sizeof(buffer) - 1, 0);
          if (recv_code <= 0) {
            printf("Client disconnected: %s\n", names[fd % NUMBER_OF_CLIENTS]);
            close(fd);
            FD_CLR(fd, &master);
          } else {
            buffer[recv_code] = '\0';
            printf("%s sent: %s\n", names[fd % NUMBER_OF_CLIENTS], buffer);
            server_logic(fd, buffer);
          }
        }
      }
    }
  }
  return 0;
}
