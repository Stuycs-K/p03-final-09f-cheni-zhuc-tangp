#include "networking.h"

void clientLogic(int server_socket){
  //passing in client which is same as server socket
  char message[256] = "";
  char response[256] = "";
  char username[32] = "";

  printf("Unnamed user: ");
  fflush(stdout);
  if (fgets(username, sizeof(username), stdin) == NULL) exit(0);
  username[strcspn(username, "\n")] = '\0';

  send(server_socket, username, sizeof(username), 0);
  recv(server_socket, username, sizeof(username), 0);
  printf("Username: %s\n", username);

  fd_set read_fds;
  while(1){
    printf("%s: ", username);
    fflush(stdout);
    
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);
    FD_SET(server_socket, &read_fds);
    select(server_socket + 1, &read_fds, NULL, NULL, NULL);


    if (FD_ISSET(server_socket, &read_fds)) {
      if (recv(server_socket, response, sizeof(response), 0) <= 0){
        printf("\nConnection lost.\n");
        break;
      }
      printf("\nrecieved: %s\n", response);
      
      if (strncmp(response, "Your name is now ", 17) == 0) {
        strcpy(username, response + 17);
      }
    }

    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      if (fgets(message, sizeof(message), stdin) == NULL) break;
      message[strcspn(message, "\n")] = '\0';
      send(server_socket, message, sizeof(message), 0);
    }
  }
  close(server_socket);
}



int main(int argc, char *argv[] ) {
  char* IP = "127.0.0.1";
  if(argc>1){
    IP=argv[1];
  }
  int server_socket = client_tcp_handshake(IP); //WHY DOES THIS GO THROUGH BUT SERVER HANDHSKAE STALLLLL;LLLL
  if(server_socket == -1) err(server_socket, "In client: ");
  //printf("client connected.\n");
  //fflush(stdout);
  clientLogic(server_socket); //stalls in this call


  //printf("server_socket_fd: %d\n", server_socket);
  //fflush(stdout);
}
