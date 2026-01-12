#include "networking.h"

void clientLogic(int server_socket){
  //passing in client which is same as server socket
  char message[256];
  char response[256];
  fd_set read_fds;

  printf("Connected to server. Commands: NAME <name>, MSG <text>, WHO, QUIT\n");

  while(1){
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);
    FD_SET(server_socket, &read_fds);

    if(select(server_socket + 1, &read_fds, NULL, NULL, NULL) == -1) {
      perror("select error");
      break;
    }

    if(FD_ISSET(server_socket, &read_fds)) {
      int recv_code = recv(server_socket, response, sizeof(response) - 1, 0);
      if(recv_code <= 0) {
        printf("Server closed.\n");
        break;
      }
      response[recv_code] = '\0';
      printf("recieved: %s\n", response);
    }

    if(FD_ISSET(STDIN_FILENO, &read_fds)) {
      if(fgets(message, sizeof(message), stdin) == NULL) break;

      message[strcspn(message, "\n")] = '\0';
      send(server_socket, message, sizeof(message), 0);
    }
  }
}



int main(int argc, char *argv[] ) {
  char* IP = "127.0.0.1";
  if(argc>1){
    IP=argv[1];
  }
  int server_socket = client_tcp_handshake(IP); 
  if(server_socket == -1) err(server_socket, "In client: ");
  
  clientLogic(server_socket); 


}
