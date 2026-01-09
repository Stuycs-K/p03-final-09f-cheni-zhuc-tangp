#include "networking.h"

void clientLogic(int server_socket){
  //passing in client which is same as server socket
  char message[256] = "";
  char response[256] = "";
  while(1)
  {
    initscr();
    cbreak();
    printw("Enter Message: \n");
    refresh();
    fflush(stdout);
    getstr(message);
    // char * fgot = fgets(message, sizeof(message), stdin);
    //
    // if (fgot == NULL){
    //   perror("Client Closed");
    //   exit(1);
    // }
    message[strcspn(message, "\n")] = '\0';
    if (strcmp(message, "exit") == 0){
      endwin();
      close(server_socket);
      exit(1);
    }
    int send_code = send(server_socket, message, sizeof(message), 0);
    if (send_code == -1) err(send_code, "In ClientLogic");

    int recv_code = recv(server_socket, response, sizeof(response), 0);
    if (recv_code == 0){
      printw("server closed\n");
      refresh();
      endwin();
      fflush(stdout);
      exit(1);
    }
    if (recv_code == -1) err(recv_code, "In ClientLogic");
    printw("received: %s\n", response);
    refresh();
    fflush(stdout);
    endwin();
    }
    close(server_socket);
    endwin();
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
