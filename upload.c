#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>

int upload_data(){

  //fork
  int to_client;
  int from_client;
  from_client = server_handshake( &to_client );
  /* DO NOT EDIT ABOVE THIS */


  /*AFTER THIS LINE:
    Try to read/write from: from_client/to_client
    The order should correspond to your basic_client.
  */

  //server_handshake will not work without cli read | expecting client to be also ran
  //both fds are written to

  //from_client is open(WKP, O_RDONLY)
  //to_client is open(pid_str, O_WRONLY)

  //from_server is open(pid_str, O_RDONLY)
  //to_server is open(WKP, O_WRONLY)
  int f = fork();
  if (f == 0){
    int message[3];
    int response[3];

    while(1){
      if (from_client == -1 || to_client == -1) {
        perror("\nser: err in handshake");
        exit(1);
      }
  while (1){
      //srand(time(NULL));
      //int rand_num = rand() % 100;
      //int bytes_wrote = write(to_client, &rand_num, sizeof(int));
      int bytes_readenen = read(from_client, message, sizeof(message));
      if (bytes_readenen == 0) {
        printf("Client disconnected\n");
        fflush(stdout);
        break;
      }
      if (bytes_readenen == -1){
        perror("\nser: err when reading");
        exit(1);
      }
      //printf("Sending %d to client\n", rand_num);
      //printf("[Subserver recieved]: %s\n", message);
      //fflush(stdout);

      //int read_rand_num_plus1;
      //srand(time(NULL));
      //int rand_num = rand() % 100;
      //int bytes_read = read(from_client, &read_rand_num_plus1, sizeof(int));
      response[0] = message[0];
      response[1] = message[1];
      response[2] = message[2];
      int bytes_wrote = write(to_client, response, sizeof(response));
      if (bytes_wrote == -1){
        perror("\nser: err when reading");
        exit(1);
      }
      //printf("[Subserver sent]: %s", response);
      //fflush(stdout);

      //signal(SIGINT, sig_handler);
    }
    close(to_client);
    close(from_client);
    exit(0);
  }
  else{
    //close(to_client);
    //close(from_client);
    printf("Ready for next client\n");
    fflush(stdout);
  }
  signal(SIGINT, sig_handler);
  }

  return 0;
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
