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
  // cbreak();
  raw(); // prevents signals from being entered
  int row = 2;
  //noecho();
  refresh();
  textbox = newwin(getmaxy(stdscr)-2, getmaxx(stdscr), 0, 0);
  mvwprintw(textbox, 1, 1, "Connected to server. Type /help for commands.\n");
  while(1){
    wborder(stdscr, '|', '|', '-', '-', '+', '+', '+', '+');
    wborder(textbox, '|', '|', '-', '-', '+', '+', '+', '+');
    for (int i = 1; i < getmaxx(stdscr)-1; i++){
      mvprintw(getmaxy(stdscr)-2, i, " ");
    }
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
      if (recv_code > 0) response[recv_code] = '\0';
      mvwprintw(textbox, row++, 1, "%s\n", response); // fix later - increment row by amount of line breaks in response
      wrefresh(textbox);
    }

    if(FD_ISSET(STDIN_FILENO, &read_fds)) {
      char * cmds[20] = {0};
      parse_args(message, cmds);

      getstr(message);
      message[strcspn(message, "\n")] = '\0';

      if (message[0] == '\0') continue;
      if (strncasecmp(message, "/help", 5) == 0){ // client side only, doesn't send to server
        mvwprintw(textbox, row, 1, "/name [user] - change the name you are displayed as\n /who - display every user connected to the server\n /quit - exit from the server\n");
        row+=3;
        wrefresh(textbox);
      } else err(send(server_socket, message, strlen(message), 0), "In client logic");
      if (strncasecmp(message, "/QUIT", 5) == 0){
        endwin();
        break; //server already knows through socket closing
      }
      if ((strcmp(cmds[0], "/UPLOAD") == 0) or (strcmp(cmds[0], "/upload") == 0)){
        int argc = get_arr_len(cmds);
        if (argc != 3){ //wrong syntax
          mvwprintw(textbox, row++, 1, "Error: Invalid syntax.\n");
          mvwprintw(textbox, row++, 1, "Usage: /upload [filepath]\n");
          wrefresh(textbox);
          continue;
        }

        //notify server of upload command
        snprintf(message, sizeof(message), "/upload %s %d\n", cmds[1], cmds[2]); 
        err(send(server_socket, message, strlen(message), 0), "In client logic");

        //handle file upload      
        char *filepath = cmds[1];
        long file_size = send_file(server_socket, filepath);
        if (file_size == -1){
          snprintf(response, sizeof(response), "Error uploading file: %s", filepath);
        } 
        else {
          snprintf(response, sizeof(response), "File uploaded successfully: %s (%ld bytes)", filepath, file_size);
        }
      }

      if ((strcmp(cmds[0], "/DOWNLOAD") == 0) or (strcmp(cmds[0], "/download") == 0)){ //not implemented yet
        int argc = get_arr_len(cmds);
        if (argc != 3){ //wrong syntax
          mvwprintw(textbox, row++, 1, "Error: Invalid syntax.\n");
          mvwprintw(textbox, row++, 1, "Usage: /download [filepath]\n");
          wrefresh(textbox);
          continue;
        }

        //maybe make sure file exists first? And easier to type path
        //handle file upload      
        char *filepath = cmds[1];
        long file_size = send_file(server_socket, filepath);
        if (file_size == -1){
          snprintf(response, sizeof(response), "Error uploading file: %s", filepath);
        } 
        else {
          snprintf(response, sizeof(response), "File uploaded successfully: %s (%ld bytes)", filepath, file_size);
        }
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
