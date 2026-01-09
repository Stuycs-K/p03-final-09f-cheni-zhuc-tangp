#include "networking.h"


void server_logic(int client_socket){
    char message[256];
    char response[256];

    while (1){
      int recv_code = recv(client_socket, message, sizeof(message) - 1, 0);
      if(recv_code > 0){
        message[recv_code] = '\0';
        //printf("socket closed\n");
        //exit(1);
      }
      if (recv_code == 0){
        printf("Client disconnected");
        fflush(stdout);
        break;
      }
      if (recv_code < 0) err(recv_code, "In subserver logic");

      char *p = message;
      char * token = strsep(&p, " ");
      if (token == NULL) {
        perror("Given empty string!");
      }
      else{ 
        if (strcmp(token, "NAME") == 0) { //how to store data?
          printf("%s\n", message);
          fflush(stdout);
          //store user
        } 
        else if (strcmp(token, "MSG ") == 0) {
          //print out in server
          printf("%s\n", message);
          fflush(stdout);
          //send to other clients
        } 
        else if (strcmp(token, "WHO ") == 0) {
          //list users
          printf("%s\n", message);
          fflush(stdout);
        } 
        else if (strcmp(token, "QUIT") == 0) {
          //probably handle in client and kicks themselves
          printf("%s\n", message);
          fflush(stdout);
        } 
        else{
          //err
          printf("%s\n", message);
          fflush(stdout);
        }
      }
    }
    close(client_socket); //not sure if this is correctly handled
}

int main(int argc, char *argv[] ) {
    int listen_socket = server_setup();
    
    socklen_t sock_size;
    struct sockaddr_storage client_address;
    sock_size = sizeof(client_address);

    fd_set read_fds, master;
    FD_ZERO(&master);
    FD_SET(listen_socket, &master);
    int max_fd = listen_socket;

    char buff[1025]="";

    while(1){
        read_fds = master;

        int readables = select(max_fd+1, &read_fds, NULL, NULL, NULL);
        err(readables, "In server main");

        // if socket
        if (FD_ISSET(listen_socket, &read_fds)) {
            //accept the connection
            int client_fd = accept(listen_socket,(struct sockaddr *)&client_address, &sock_size);
            err(client_fd, "In server Main");
            
            printf("Connected, waiting for data.\n");
            fflush(stdout);

            FD_SET(client_fd, &master);
            if (client_fd > max_fd) max_fd = client_fd;

            printf("\nRecieved from client_fd %d: %s\n",client_fd, buff);
            fflush(stdout);

            if (--readables == 0) continue;
        }

        for (int fd = 0; fd <= max_fd; fd++){
          if (fd == listen_socket) continue;
          if (!FD_ISSET(fd, &read_fds)) continue;

          char buf[256];
          int recv_code = recv(fd, buf, sizeof(buf) -1, 0);
          
          if (recv_code == 0){ //Client's Socket closed
            printf("Client disconnected: fd = %d\n", fd);
            fflush(stdout);
            close(fd);
            FD_CLR(fd, &master);
          }
          else if (recv_code < 0){ //err
            err(recv_code, "In Server Main");
            close(fd);
            FD_CLR(fd, &master);
          }
          else{ //data
            buf[recv_code] = '\0';
            printf("%s\n", buf); //replace with logic function
            fflush(stdout);
          }

          if (--readables == 0) break;
        }
    }

    //int client_socket = server_tcp_handshake(listen_socket); 
    //if (client_socket == -1) err(client_socket, "In server main");

    //printf("successful server handshake\n");
    //fflush(stdout);

    //server_logic(client_socket);
    return 0;
}