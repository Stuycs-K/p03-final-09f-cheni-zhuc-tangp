# Dev Log:

This document must be updated daily every time you finish a work session.

## Clayton Zhu

### 2026-01-06 - Looked at where propsed changes can fit into lab16 framework, Decide on architecture for handler ->parent-> other handlers, unworking implementation of such
- Looking at changes in lab16 framework: 20 minutes, Reminding self of lab16 and moving variables outside fork loop </br>
- Deciding on architecture: 40 mins, Initially wanted to do semaphores with clients writiing in turns but I don't wanna do semaphores | Also easier to store client fds,  transport that to handler then parent, and then distribute to all handlers </br>
- Unworking implementation: 10 min, Looked over lab15 on how to do 2-way pipes </br>

### 2026-01-07 - Updated proposal, worked on select()
- Updated proposal + readme: 45 minutes, Spent most of time thinking ahead of how to do upload and download
- Worked on Select(): 40 minutes, cut fork server with subserver logic, tried to figure out where to put fd building and setting function
- tested Patrick's code: 10 minutes, no compile error but select was used after recv

### 2026-1-08 - Resolved errs, Infrastr for dealing with multiple client fds
- Resolved err: 1 hour, finding why server prints wrong after cli disconnects, bad fd from redeclaring fd
- Multiple Client fds hadnling: 1.5 hours, replaced tcp handshake with select infrastructure including handling new connections and iterating through fd list for readable sockets

### 2026-1-09 - Tested concurrent reading and started writing back
- Tested concurrent: 15 min reading in server using tmux
- Started writing back: 20 min, translating the reading and its iterators over fd to writing

### 2026-1-11 - Tested concurrent reading and writing and wrote back
- Tested concurrent: 20 min used telnet server_ip port for local, lab, lab + telnet
- Started writing back: 15 min, iterated through fd set to send back to clients

### 2026-1-12 - Handled commands, worked on file_transfer
- Handled commands: 40 min less buggy way to handle MSG, NAME, QUIT. Replaced by Patrick's version
- File transfer: 40 min, send_all and recieve_all loops while sending/recieving until no bytes left. Send file function

### 2026-1-13 - Researched ncurses, tried to fix terminal buffer problem
- Research ncurses: 30 min read over docu from link on repo
- terminal buffer: 30 min created new window but didn't work


### 2026-1-14 - Added File transfer
- Designing file transfer: 30 min , tried several different ideas including making exclsuive socket connection for every file transfer and using syn acks to sync.
- Added File transfer helpers: 10 min Parsing args for more than 1 parameter using arg_parse 
- Building and rebuilding file transfer: 2 hours, could not figure out how to send uninterrupted file sends, reworked send and recieve_file with byte size, send command separate from file