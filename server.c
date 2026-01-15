#include "networking.h"
#include "upload.h"

#define NUMBER_OF_CLIENTS 100
char names[NUMBER_OF_CLIENTS][256];

struct file_info server_files[MAX_FILES];
int file_count = 0;

void add_file_info_to_list(char *filepath, long size) {
  for (int i = 0; i < file_count; i++) {
    if (strcmp(server_files[i].filename, filepath) == 0) {
      server_files[i].size = size;
      return;
    }
  }

  if (file_count < MAX_FILES) {
    strncpy(server_files[file_count].filename, filepath, 255);
    server_files[file_count].size = size;
    file_count++;
  }
}

int listen_socket;
fd_set master;
int max_fd;

static void sighandler(int signo) {
  if(signo == SIGINT || signo == SIGTERM){
    printf("\nServer shutting down\n");
    char *msg = "Server shut down\n";
    for(int i = 0; i <= max_fd; i++){
      if(FD_ISSET(i, &master)){
        if(i != listen_socket){
          send(i, msg, strlen(msg), 0);
        }
        close(i);
      }
    }
    exit(0);
  }
}



void server_logic(int fd, char * message, fd_set * master, int max_fd, int listen_socket) {
  char response[BUFFER_SIZE] = {0};
  char * cmds[20] = {0};
  parse_args(message, cmds);

  if (strncmp(message, "/", 1) == 0){
    if (strncasecmp(message, "/LIST", 5) == 0){
      if (file_count == 0){
        strcpy(response, "No files available on sever.\n");
      } else {
        for (int i = 0; i < file_count; i++){
          char file_line[512];
          snprintf(file_line, sizeof(file_line), "%d. %s (%ld bytes)\n", i+1, server_files[i].filename, server_files[i].size);
          strncat(response, file_line, sizeof(response) - strlen(response) - 1);
        }
      }

      send(fd, response, strlen(response), 0);
      return;
    }

    if ((strcasecmp(cmds[0], "/UPLOAD") == 0)){
      if (get_arr_len(cmds) != 3){
        send(fd, "ERROR\n", 6, 0);
        return;
      }

      char *filepath = cmds[1];
      long file_size = atol(cmds[2]); //str to long

      send(fd, "ACK\n", 4, 0);

      long received = receive_file(fd, filepath, file_size);

      if (file_size == -1){
        snprintf(response, sizeof(response), "Error uploading file: %s", filepath);
      }
      else {
        snprintf(response, sizeof(response), "Uploaded successfully: %s (%ld bytes)", filepath, file_size);
        add_file_info_to_list(filepath, file_size);
      }

      send(fd, response, strlen(response), 0);
      return;
    }

    if (strncasecmp(message, "/DOWNLOAD ", 10) == 0){
      char *filepath = message + 10;

      //get file size
      int found = -1;
      for (int i = 0; i < file_count; i++){
        if (strcmp(server_files[i].filename, filepath) == 0){
          found = i;
          break;
        }
      }

      if (found == -1){
        send(fd, "ERROR\n", 6, 0);
        return;
      }

      long file_size = server_files[found].size; //atol?
      //char size_msg[64];

      snprintf(response, sizeof(response), "READY %s %ld", server_files[found].filename, file_size);
      send(fd, response, strlen(response), 0);

      char ack[256];
      recv(fd, ack, sizeof(ack) - 1, 0); //wait for client ack so pure, blue-eyed, blonde files

      long sent = send_file(fd, server_files[found].filename);

      //snprintf(size_msg, sizeof(size_msg), "%ld\n", file_size);

      if (sent == -1){
        snprintf(response, sizeof(response), "Error sending file: %s", filepath);
      } else {
        snprintf(response, sizeof(response), "File transferred successfully: %s (%ld bytes)", server_files[found].filename, sent);
      }

      send(fd, response, strlen(response), 0);
      return;
    }
    if(strncasecmp(message, "/NAME ", 6) == 0){
      strncpy(names[fd % NUMBER_OF_CLIENTS], message + 6, 255);
      snprintf(response, sizeof(response), "Renamed to: %s", names[fd % NUMBER_OF_CLIENTS]);
      send(fd, response, strlen(response), 0);
      return;
    }
    if (strncasecmp(message, "/WHO", 4) == 0) {
      strcpy(response, "Active users: ");
      for(int i = 0; i <= max_fd; i++) {
        if(FD_ISSET(i, master) && i != listen_socket) {
          strncat(response, "\n", 2);
          strncat(response, names[i % NUMBER_OF_CLIENTS], sizeof(response) - strlen(response) - 1);
          strncat(response, " ", sizeof(response) - strlen(response) - 1);
        }
      }
      send(fd, response, strlen(response), 0);
      return;
    }
    if (strncasecmp(message, "/QUIT", 5) == 0) {
      strcpy(response, "Quitting...");
      send(fd, response, strlen(response), 0);
      close(fd);
      FD_CLR(fd, master);
      return;
    }
  } else{
    snprintf(response, sizeof(response), "[%s]: %s", names[fd % NUMBER_OF_CLIENTS], message);
    send(fd, response, strlen(response), 0);
  }

  //loop through all fd here
  for (int i = 0; i <= max_fd; i++) {
    if (FD_ISSET(i, master)) {
      if (i != listen_socket && i != fd) { //add i != fd if repeating msg
        send(i, response, strlen(response), 0);
        }
      }
    }
  }




int main(int argc, char *argv[] ) {
 // struct client clients[NUMBER_OF_CLIENTS];
  signal(SIGINT, sighandler);
  signal(SIGTERM, sighandler);

  listen_socket = server_setup();
   //for(int i=0; i<NUMBER_OF_CLIENTS; i++) strcpy(clients[i].name, "Unnamed");

  fd_set read_fds;
  FD_ZERO(&master);
  FD_SET(listen_socket, &master);
  max_fd = listen_socket;

  printf("Server started\n");

  while(1){
    read_fds = master;
    if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1) {
      perror("select error");
      continue;
    }

    for (int fd = 0; fd <= max_fd; fd++) { //for all fds
      if (FD_ISSET(fd, &read_fds)) { //fd is in set
        if (fd == listen_socket) {
          int client_fd = server_tcp_handshake(listen_socket);
          if (client_fd != -1) {
            //FD_CLR(fd, &master); //remove listen socket and add client socket
            FD_SET(client_fd, &master); //add fd to master
            if (client_fd > max_fd) max_fd = client_fd;
            printf("client fd %d connected\n", client_fd);
          }
        }
        else {
          char buffer[BUFFER_SIZE];
          int recv_code = recv(fd, buffer, sizeof(buffer) - 1, 0);
          if (recv_code <= 0) {
            //printf("Client disconnected: %s\n", clients[fd % NUMBER_OF_CLIENTS].name);
            printf("Client disconnected: %s\n", names[fd % NUMBER_OF_CLIENTS]);
            close(fd);
            FD_CLR(fd, &master);
          }
          else {
            buffer[recv_code] = '\0';
            buffer[strcspn(buffer, "\r\n")] = '\0';

            printf("%s sent: %s\n", names[fd % NUMBER_OF_CLIENTS], buffer);
            //server_logic(fd, buffer, &master, max_fd, listen_socket, clients);
            server_logic(fd, buffer, &master, max_fd, listen_socket);
          }
        }
      }
    }
  }
  return 0;
}
