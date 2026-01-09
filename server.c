#include "networking.h"


void server_logic(int client_socket){
    char message[256];
    char response[256];

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
      else{ 
        if (strcmp(token, "NAME ") == 0) { //how to store data?
          printf("%s\n", message);
          fflush(stdout);
          //store user
        } 
        else if (strcmp(token, "MSG ") == 0) {
          //print out in server
          printf("%s\n", message);
          fflush(stdout);
          //send to other clients
        } 
        else if (strcmp(token, "WHO ") == 0) {
          //list users
          printf("%s\n", message);
          fflush(stdout);
        } 
        else if (strcmp(token, "QUIT") == 0) {
          //probably handle in client and kicks themselves
          printf("%s\n", message);
          fflush(stdout);
        } 
        else{
          //err
          printf("%s\n", message);
          fflush(stdout);
        }
      }
    }
    close(client_socket); //not sure if this is correctly handled
}

int main(int argc, char *argv[] ) {
  int listen_socket = server_setup();
  while(1){
    int client_socket = server_tcp_handshake(listen_socket); // stalls here
    if (client_socket == -1) err(client_socket, "In server main");
    printf("successful server handshake\n");
    fflush(stdout);

    server_logic(client_socket);
  
  }
}