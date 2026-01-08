#include "networking.h"

void clientLogic(int server_socket){
  //passing in client which is same as server socket
  char message[256] = "";
  char response[256] = "";
  char username[256] = "";

  printf("Enter username: ");
  fflush(stdout);
  if (fgets(username, sizeof(username), stdin) == NULL) exit(0);
  username[strcspn(username, "\n")] = '\0';

  int send_code = send(server_socket, username, sizeof(username), 0);
  if (send_code == -1) err(send_code, "In ClientLogic");

  int recv_code = recv(server_socket, username, sizeof(username), 0);
  if (recv_code == 0){
    printf("socket closed\n");
    fflush(stdout);
    exit(1);
  }
  if (recv_code == -1) err(recv_code, "In ClientLogic");
  printf("Name: %s\n", username);
  fflush(stdout);

  fd_set read_fds;
  while(1){
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);
    FD_SET(server_socket, &read_fds);
    select(server_socket + 1, &read_fds, NULL, NULL, NULL);
    printf("%s: ", username);
    fflush(stdout);

    if (FD_ISSET(server_socket, &read_fds)) {
      if (recv(server_socket, response, sizeof(response), 0) <= 0) break;
      printf("%s\n", response);
    }

    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      fgets(message, sizeof(message), stdin);
      message[strcspn(message, "\n")] = '\0';
      send(server_socket, message, sizeof(message), 0);
    }

    char * fgot = fgets(message, sizeof(message), stdin);
    if (fgot == NULL){
      perror("Client Closed");
      exit(1);
    }
    message[strcspn(message, "\n")] = '\0';

    //int bytes_wrote = write(server_socket, message, strlen(message)); //stalls here
    //if (bytes_wrote == -1) err(bytes_wrote, "In clientLogic: ");
    int send_code = send(server_socket, message, sizeof(message), 0);
    if (send_code == -1) err(send_code, "In ClientLogic");
    //printf("ckpt in client sublogic");
    //fflush(stdout);

    //int bytes_read = read(server_socket, response, sizeof(response));
    //if (bytes_read == -1) err(bytes_read, "in clientLogic: ");
    int recv_code = recv(server_socket, response, sizeof(response), 0);
    if (recv_code == 0){
      printf("socket closed\n");
      fflush(stdout);
      exit(1);
    }
    if (recv_code == -1) err(recv_code, "In ClientLogic");
    printf("received: %s\n", response);
    fflush(stdout);
    //sleep(1);
    //read(from_server, &clear_read_buff, sizeof(int)); //clear pipe
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
  
  clientLogic(server_socket); 


}