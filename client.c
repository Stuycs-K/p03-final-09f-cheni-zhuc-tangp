#include "networking.h"

void clientLogic(int server_socket){
  //passing in client which is same as server socket
  char message[256];
  char response[256];

  while(1){
    printf("Enter Message: \n");
    fflush(stdout);

    char * fgot = fgets(message, sizeof(message), stdin);
    if (fgot == NULL){
      perror("Client Closed");
      exit(1);
    }
    message[strcspn(message, "\n")] = '\0';
    int send_code = send(server_socket, message, sizeof(message), 0);
    if (send_code == -1) err(send_code, "In ClientLogic");

  //int bytes_read = read(server_socket, response, sizeof(response));
  //if (bytes_read == -1) err(bytes_read, "in clientLogic: ");

  //printf("[Client received]: %s\n", response);
  //fflush(stdout);
  //sleep(1);
  }
  close(server_socket);

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
