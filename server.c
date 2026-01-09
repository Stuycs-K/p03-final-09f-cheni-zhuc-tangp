#include "networking.h"



void server_logic(int client_socket){
    //remove forking and child
    int f = fork();

    if (f == 0){ //child
      char message[256] = "";
      char response[256] = "";


      while (1){

        int recv_code = recv(client_socket, message, sizeof(message), 0);
        if(recv_code == 0){
          printf("socket closed\n");
          exit(1);
        }
        if (recv_code == -1) err(recv_code, "In subserver logic");

        char * token = strsep(&message, " ");
        if (token == NULL) {
          perror("Given empty string!");
        }
        else{ //come back for what to do with commands
          if (strcmp(token, "NAME ") == 0) {
            strcpy(response, message + 5);
          } 
          else if (strncmp(message, "MSG ", 4) == 0) {
            strcpy(response, message + 4);
          } 
          else if (strncmp(message, "WHO", 3) == 0) {

          } 
          else if (strncmp(message, "QUIT", 4) == 0) {

          } 
          else strcpy(response, "Invalid command");

          printf("%s\n", message);
          int send_code = send(client_socket, response, sizeof(response), 0);
          if (send_code == -1) err(send_code, "In subserver logic");
        }
      }
      exit(0);
    }
    else{
      close(client_socket);
      }
}

int main(int argc, char *argv[] ) {
  int listen_socket = server_setup();
  while(1){
    int listen_socket = server_tcp_handshake(listen_socket); // stalls here
    if (listen_socket == -1) err(listen_socket, "In server main");
    printf("successful server handshake\n");
    fflush(stdout);

    server_logic(listen_socket);
  
  }
}