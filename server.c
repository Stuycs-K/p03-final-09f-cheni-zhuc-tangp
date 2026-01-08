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
    int client_socket = server_tcp_handshake(listen_socket); // stalls here
    if (client_socket == -1) err(client_socket, "In server main");
    printf("successful server handshake\n");
    fflush(stdout);

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
      printf("Connected, waiting for data.\n");
      
      //read the whole buff
      read(client_socket, buff, sizeof(buff));
      //trim the string
      buff[strlen(buff)-1]=0; //clear newline
      if(buff[strlen(buff)-1]==13){
          //clear windows line ending
          buff[strlen(buff)-1]=0;
      }

      printf("\nRecieved from client '%s'\n",buff);
      close(client_socket);
  }
  
  }
}