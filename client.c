#include "networking.h"


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
  // scrollok(stdscr, TRUE);
  scrollok(textbox, TRUE);
  textbox = newwin(getmaxy(stdscr)-4, getmaxx(stdscr)-2, 1, 1);
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
      if (recv_code > 0) response[recv_code] = '\0';
      wprintw(textbox, "%s\n", response);
      // scroll(textbox);
      wrefresh(textbox);
    }

    if(FD_ISSET(STDIN_FILENO, &read_fds)) {
      getstr(message);
      message[strcspn(message, "\n")] = '\0';
      if (message[0] == '\0') continue;
      if (strncasecmp(message, "/help", 5) == 0){ // client side only, doesn't send to server
        wprintw(textbox, "/name [user] - change the name you are displayed as\n/who - display every user connected to the server\n/quit - exit from the server\n");
        wrefresh(textbox);
      }else err(send(server_socket, message, strlen(message), 0), "In client logic");

      if (strncasecmp(message, "/QUIT", 5) == 0){
        endwin();
        break; //server already knows through socket closing
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
