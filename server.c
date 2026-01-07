#include "networking.h"


char * rot13(char*s){
  int x = 13;

  for (int i = 0; i < strlen(s); i++){
    if (s[i] - 'a' < 26 && s[i] - 'a' >= 0){ //if diff in range then letter is lowercase
      if (s[i] + x > 'z'){
        s[i] = s[i] + x - 26;
      }
      else{
        s[i] = s[i] + x;
      }
    }
    else if (s[i] - 'A' < 26 && s[i] - 'A' >= 0){ //if diff in range then letter is uppercase
      if (s[i] + x > 'Z'){
        s[i] = s[i] + x - 26;
      }
      else{
        s[i] = s[i] + x ;
      }
    }
  }
  return s;
}

void subserver_logic(int client_socket){
    int to_parent[2];
    if (pipe(to_parent) == -1) err(to_parent[0], "In subserver_logic");

    int to_child[2];
    if (pipe(to_child) == -1) err(to_child[0], "In subserver_logic");

    int f = fork();

    if (f == 0){ //child
      char message[256] = "";
      char response[256] = "";

      close(to_parent[0]);
      close(to_child[1]);

      while (1){
        int recv_code = recv(client_socket, message, sizeof(message), 0);
        if(recv_code == 0){
          printf("socket closed\n");
          exit(1);
        }
        if (recv_code == -1) err(recv_code, "In subserver logic");

        //strcpy(response, rot13(message));

        //int send_code = send(client_socket, response, sizeof(response), 0);
        //if (send_code == -1) err(send_code, "In subserver logic");
      }
      exit(0);
    }
    else{ //parent
      close(to_parent[1]);
      close(to_child[0]);

      close(client_socket);
      }
}

int main(int argc, char *argv[] ) {
  int fds[100]; //store all client fds
  int count = 0;

  int listen_socket = server_setup();
  while(1){
    int client_socket = server_tcp_handshake(listen_socket); 
    if (client_socket == -1) err(client_socket, "In server main"); //maybe build socket fd set here 
    fds[count++] = client_socket;

    printf("successful server handshake\n");
    fflush(stdout);

    /*
    Add storage for socket fd to send client msg across, what if new person joins? Bc forked process has copy of old socket fd only
    */
    subserver_logic(client_socket); //client_socket is server socket

  }
}
