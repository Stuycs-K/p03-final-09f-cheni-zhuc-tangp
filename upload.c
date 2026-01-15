#include "networking.h"

void parse_args( char * line, char ** arg_ary ){
  int count = 0;
  char * token;
  char * line_copy = strdup(line);
  char * line_tmp = line_copy;
  while(token = strsep(&line_tmp, " ")){ //NULL is false
    //strip_quotes(token);
    if (token && *token){
      //if (strcmp(token, "" z))
      arg_ary[count++] = token;
    }
  }
  arg_ary[count] = NULL;
}

int get_arr_len(char ** arr){
  int count = 0;
  while(arr[count] != NULL){
    count++;
  }
  return count;
}

long send_file(int socket, char * filepath){
  struct stat stat_buffer; 
  char file_buffer[BUFFER_SIZE]; 
  size_t n_read;
  err((stat(filepath, &stat_buffer) == -1), "in send_file");

  long file_size = stat_buffer.st_size;
  
  FILE *file = fopen(filepath, "rb");
  if (!file) {
    perror("fopen error");
    return -1;
  }

  printf("Sending file: %s (%ld bytes)\n", filepath, file_size);

  while(n_read = fread(file_buffer, 1, BUFFER_SIZE, file)) {
    //while(total < n_read) 
    if (n_read > 0){
      size_t total = 0;
      while (total < n_read){ //works bc buffer_size items, all of which are 1  
        size_t bytes_sent = send(socket, file_buffer + total, n_read - total, 0);

        if (bytes_sent < 0){
          fclose(file);
          perror("Send file error");
          return -1;
        }

        if (bytes_sent == 0){
          fclose(file);
          return -1;
        }
        total += bytes_sent; 
      }
    }

    if (n_read < sizeof(file_buffer)){ //end of file
      break;
    }
  }
  fclose(file);
  return file_size;
}

//modify recieve_file
int receive_file(int socket, char * filepath, long file_size){
  long remaining = file_size;
  char file_buffer[BUFFER_SIZE];

  strcat(filepath, ".received"); //otherwise overwrites existing file on local
  FILE *file = fopen(filepath, "wb"); 
  if (!file) {
    perror("fopen error");
    return -1;
  }

  //printf("Recving file: %s (%ld bytes)\n", filepath, file_size);

  while(remaining > 0) {
    long to_recv = (remaining < BUFFER_SIZE ? remaining : BUFFER_SIZE); //we don't have min?
    size_t n_wrote = recv(socket, file_buffer, to_recv, 0);

    if (n_wrote <= 0){
      fclose(file);
      perror("Receive file error");
      return -1;
    }
    
    fwrite(file_buffer, 1, n_wrote, file); //works bc buffer_size items, all of which are 1  
    remaining -= n_wrote;
  }

  fclose(file);
  return file_size;
}
