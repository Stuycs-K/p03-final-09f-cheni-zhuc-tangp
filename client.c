#include "networking.h"
#include "upload.h"

void clientLogic(int server_socket){
  char message[256];
  char response[256];
  char buffer[BUFFER_SIZE];
  fd_set read_fds;
  WINDOW * textbox;
  initscr();
  wborder(stdscr, '|', '|', '-', '-', '+', '+', '+', '+');
  cbreak();
  // raw(); // prevents signals from being entered
  int row = 2;
  //noecho();
  refresh();
  textbox = newwin(getmaxy(stdscr)-4, getmaxx(stdscr)-2, 1, 1);
  scrollok(textbox,TRUE);
  wprintw(textbox, "Connected to server. Type /help for commands.\n");
  while(1){
    wborder(stdscr, '|', '|', '-', '-', '+', '+', '+', '+');
    mvwin(textbox, 1, 1); // doesn't actually move, but sends to the top
    for (int i = 1; i < getmaxx(stdscr)-1; i++){
      mvprintw(getmaxy(stdscr)-3, i, "-");
      mvprintw(getmaxy(stdscr)-2, i, " ");
    }
    mvprintw(getmaxy(stdscr)-3, 0, "+");
    mvprintw(getmaxy(stdscr)-3, getmaxx(stdscr)-1, "+");
    refresh();
    wrefresh(textbox);
    move(getmaxy(stdscr)-2, 1);
    FD_ZERO(&read_fds);
    FD_SET(STDIN_FILENO, &read_fds);
    FD_SET(server_socket, &read_fds);

    if(select(server_socket + 1, &read_fds, NULL, NULL, NULL) == -1) {
      perror("select error");
      endwin();
      break;
    }

    if(FD_ISSET(server_socket, &read_fds)) {
      int recv_code = recv(server_socket, response, sizeof(response) - 1, 0);
      err(recv_code, "In cli logic");
      if (recv_code == 0){
        wprintw(textbox, "Server disconnected.\n");
        wrefresh(textbox);
        endwin();
        exit(1);
      }
      if (recv_code > 0) response[recv_code] = '\0';
      wprintw(textbox, "%s", response); 
      wrefresh(textbox);
    }

    if(FD_ISSET(STDIN_FILENO, &read_fds)) {
      getstr(message);
      message[strcspn(message, "\n")] = '\0';

      char * cmds[20] = {0};
      parse_args(message, cmds);


      if (message[0] == '\0') continue;
      if (strncasecmp(message, "/help", 5) == 0) {
        wprintw(textbox, "/name [user] - change the name you are displayed as\n");
        wprintw(textbox, "/who - display every user connected to the server\n");
        wprintw(textbox, "/upload [filepath] - upload a file to the server\n");
        wprintw(textbox, "/list - view available files on server\n");
        wprintw(textbox, "/download [filename] - download a file from server\n");
        wprintw(textbox, "/quit - exit from the server\n");
        row += 6;
        wrefresh(textbox);
      }
      else if (strcasecmp(message, "/LIST") == 0){
        send(server_socket, message, strlen(message), 0);
      }
      else if (strncasecmp(message, "/QUIT", 5) == 0){
        send(server_socket, message, strlen(message), 0);
        endwin();
        break; //server already knows through socket closing
      }
      else if ((strcasecmp(cmds[0], "/UPLOAD") == 0)){
        int argc = get_arr_len(cmds);
        if (argc != 2){ 
          wprintw(textbox, "Error: Invalid syntax.\n");
          wprintw(textbox, "Usage: /upload [filepath]\n");
          wrefresh(textbox);
        }

        struct stat stat_buffer;
        if (stat(cmds[1], &stat_buffer) == -1){
          wprintw(textbox, "Error: File does not exist: %s\n", cmds[1]);
          wrefresh(textbox);
          message[0] = '\0'; //clear the message, does not work
        }

        snprintf(message, sizeof(message), "/upload %s %ld\n", cmds[1], stat_buffer.st_size);
        err(send(server_socket, message, strlen(message), 0), "In client logic"); //let server know file is coming

        char ack[256];
        int ack_len = recv(server_socket, ack, sizeof(ack) - 1, 0); //wait for server ack so pure, blue-eyed, blonde files
        ack[ack_len] = '\0';

        long sent = send_file(server_socket, cmds[1]); //actual while loop sending
        //if (sent == -1){
        //  wprintw(textbox, "Error uploading file: %s\n", cmds[1]);
        //}
        //else {
        //  wprintw(textbox, "File uploaded successfully: %s (%ld bytes)\n", cmds[1], sent);
        //}
        //wrefresh(textbox);
      }
      else if ((strcasecmp(cmds[0], "/DOWNLOAD") == 0)){ //not implemented yet
        int argc = get_arr_len(cmds);
        if (argc != 2){ //wrong syntax
          wprintw(textbox, "Usage: /download [filepath]\n");
          wrefresh(textbox);
          continue;
        }

        snprintf(message, sizeof(message), "/download %s\n", cmds[1]);
        err(send(server_socket, message, strlen(message), 0), "In client logic");

        char response[256];
        int response_code = recv(server_socket, response, sizeof(response) - 1, 0);
        response[response_code] = '\0';

        char * cmds2[20] = {0};
        parse_args(response, cmds2);

        if (strcmp(cmds2[0], "READY") == 0) {
          char *filename = cmds2[1];
          long file_size = atol(cmds2[2]); //str t long

          send(server_socket, "ACK\n", 4, 0);

          long received = receive_file(server_socket, filename, file_size);

          if (received == -1){
            wprintw(textbox, "Error downloading file: %s\n", filename);
          }
          else {
            wprintw(textbox, "File downloaded successfully: %s (%ld bytes)\n", filename, received);
          }
          wrefresh(textbox);
        }
        continue;
      }
      else {
        err(send(server_socket, message, strlen(message), 0), "In client logic");
      }
  }
}
close(server_socket);
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