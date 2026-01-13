#include <sys/stats.h>
#include <fcntl.h>

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
  struct stat stat_buffer;
  err((stat(filepath, &stat_buffer) == -1), "in send_file");

  long file_size = stat_buffer.st_size;
  
  FILE *file = fopen(filepath, "rb");
  if (!file) {
    perror("fopen error");
    return -1;
  }

  printf("Sending file: %s (%ld bytes)\n", filepath, file_size);

  if (send_all(socket, , sizeof(file_size)) == -1) {
    fclose(file);
    return -1;
  }
}

int recieve_file(){

}
