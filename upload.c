#include <sys/stats.h>

int send_all(int socket, char * buffer, int length){
  //get file size using stat then keep sending until match

  int total_sent = 0;
  int bytes_left = length;  
  int n;

  while (total_sent < length){
    n = send(socket_fd, buffer + total_sent, bytes_left, 0); //shift first index, big container
    err(n, "In send all");
    total_sent += n;
    bytes_left -= n;
  }

  return total_sent
}

int recv_all(int socket, char * buffer, int length){
  //get file size using stat then keep recv until match

  int total_recv = 0;
  int bytes_left = length;  
  int n;

  while (total_recv < length){
    n = recv(socket_fd, buffer + total_recv, bytes_left, 0); //shift first index, big container
    err(n, "In send all");
    if (n == 0){
      return 0; //from server pov, cli closed
    }

    total_recv += n;
    bytes_left -= n;
  }

  return total_recv;
}

int send_file(int socket, char * filepath){
  
}

int recieve_file(){
  
}