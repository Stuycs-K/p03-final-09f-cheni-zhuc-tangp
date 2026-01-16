[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/QfAIDTGi)
# Mutli-Client Chatroom Server

### Intercontenental Policing Committee

Clayton Zhu, Patrick Tang, Ivan Chen

### Project Description:

We will build a terminal-based, multi-client chat system with a central server. Multiple clients can connect concurrently, choose usernames, and send messages that are broadcast to everyone. Additionally clients will be able to transfer large files to and from the server.

### Instructions:

To compile, do 'make compile'. Server is ran with ./server, client is ran with ./client IP
The user interacts with the program through COMMAND ARG:
- /name [user] - change the name you are displayed 
- /who - display every user connected to the server
- /upload [filepath] - upload a file to the server
- /list - view available files on server
- /download [filename] - download a file from server
- /quit - exit from the server

### Resources/ References:
- Ncurses 
- Beej's guide
- konstantinnovation.github.io
- https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/ 

### Video Presentation Link

https://www.youtube.com/watch?v=geNLJPPpDl8
