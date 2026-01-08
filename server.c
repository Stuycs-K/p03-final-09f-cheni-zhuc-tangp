#include "networking.h"


char * rot13(char*s){
  // int x = 13;

  // for (int i = 0; i < strlen(s); i++){
  //   if (s[i] - 'a' < 26 && s[i] - 'a' >= 0){ //if diff in range then letter is lowercase
  //     if (s[i] + x > 'z'){
  //       s[i] = s[i] + x - 26;
  //     }
  //     else{
  //       s[i] = s[i] + x;
  //     }
  //   }
  //   else if (s[i] - 'A' < 26 && s[i] - 'A' >= 0){ //if diff in range then letter is uppercase
  //     if (s[i] + x > 'Z'){
  //       s[i] = s[i] + x - 26;
  //     }
  //     else{
  //       s[i] = s[i] + x ;
  //     }
  //   }
  // }
  return s;
}


//void sig_handler(int signo){
//  err(signo, "Caught signal: ");
//  exit(1);
//}

void subserver_logic(int client_socket){
    int f = fork();
    //printf("ckpt in subserver logic");
    //fflush(stdout);
    if (f == 0){ //child
      char message[256] = "";
      char response[256] = "";


      while (1){
        //int bytes_read = read(client_socket, message, sizeof(message));
        //if (bytes_read == 0) {
        //  err(client_socket, "In server sublogic: ");
        //  exit(1);
        //}
        int recv_code = recv(client_socket, message, sizeof(message), 0);
        if(recv_code == 0){
          printf("socket closed\n");
          exit(1);
        }
        if (recv_code == -1) err(recv_code, "In subserver logic");

        //modify response with rot13
        if (strncmp(message, "NAME ", 5) == 0) {
          strcpy(response, message + 5);
        } else if (strncmp(message, "MSG ", 4) == 0) {
          strcpy(response, message + 4);
        } else if (strncmp(message, "WHO", 3) == 0) {
          strcpy(response, "Server list");
        } else if (strncmp(message, "QUIT", 4) == 0) {
          strcpy(response, "Quitting");
          exit(0);
        } else strcpy(response, "Invalid command");
        //response[bytes_read] = '\0';
        //int bytes_wrote = write(client_socket, response, sizeof(response));
        //if (bytes_wrote == -1) {
        //  err(bytes_wrote, "In server sublogic: ");
        //  exit(1);
        //}
        printf("%s\n", message);
        int send_code = send(client_socket, response, sizeof(response), 0);
        if (send_code == -1) err(send_code, "In subserver logic");

        //signal(SIGINT, sig_handler);
      }
      exit(0);
    }
    else{
      //printf("Ready for next client\n");
      //fflush(stdout);
      close(client_socket);
      }
}

int main(int argc, char *argv[] ) {
  int listen_socket = server_setup();
  while(1){
    //printf("ckpt before server handshake\n"); //does not print but client still returns
    //fflush(stdout);
    int client_socket = server_tcp_handshake(listen_socket); // stalls here
    if (client_socket == -1) err(client_socket, "In server main");
    printf("successful server handshake\n");
    fflush(stdout);
    //printf("client_socket_fd: %d\n", client_socket);
    //fflush(stdout);

    subserver_logic(client_socket); //client_socket is server socket

  }
}
