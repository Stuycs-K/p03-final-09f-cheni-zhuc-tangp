#include "networking.h"

void clientLogic(int server_socket){
  char message[256];
  char response[256];
  char buffer[BUFFER_SIZE];
  fd_set read_fds;
  int row = 1;
  initscr();
  wborder(stdscr, '|', '|', '-', '-', '+', '+', '+', '+');
  cbreak();
  mvprintw(row++, 1, "Connected to server. Type /help for commands.\n");
  refresh();
  while(1){
    wborder(stdscr, '|', '|', '-', '-', '+', '+', '+', '+');
    for (int i = 1; i < getmaxx(stdscr)-2; i++){
      mvprintw(getmaxy(stdscr)-2, i, " ");
    }
    move(getmaxy(stdscr)-2, 1);
    refresh();
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
      refresh();
      if (recv_code > 0) response[recv_code] = '\0';
      mvprintw(row++, 1, "recieved: %s\n", response);
      refresh();
    }

    if(FD_ISSET(STDIN_FILENO, &read_fds)) {
      // if(fgets(message, sizeof(message), stdin) == NULL) break;
      getstr(message);
      message[strcspn(message, "\n")] = '\0';
      if (message[0] == '\0') continue;

      err(send(server_socket, message, strlen(message), 0), "In client logic");

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
