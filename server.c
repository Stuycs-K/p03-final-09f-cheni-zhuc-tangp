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

    char message[256] = "";
    char response[256] = "";

    int f = fork();
    if (f == 0){ //child
      close(to_parent[0]);
      close(to_child[1]);

      while (1){
        int recv_code = recv(client_socket, message, sizeof(message), 0);
        if(recv_code == 0){
          printf("client exit\n");
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

      char message[256] = "";
      char response[256] = "";

      int bytes_wrote = write(to_parent[0], message, strlen(message)); //up to parent to redistribute
      if (bytes_wrote == -1) err(bytes_wrote, "In subserver logic");

      close(client_socket);
    }
}

int main(int argc, char *argv[] ) {
  int fds[100] = {0}; //store all client fds
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

    int bytes_read = read(fds[0], message, sizeof(message)); //from first client for now
    if (bytes_read == -1) err(bytes_read, "In main server logic");
    //distribute to all clients
    for (int i = 0; i < count; i++){
      int bytes_wrote = write(fds[i], message, strlen(message));
      if (bytes_wrote == -1) err(bytes_wrote, "In main server logic");
    }
  }
}
