# Final Project Proposal

# Group Members:
- Clayton Zhu
- Patrick Tang
- Ivan Chen 

## Intentions:
We will build a terminal-based, multi-client chat system with a central server. Multiple clients can connect concurrently, choose usernames, and send messages that are broadcast to everyone. Additionally clients will be able to transfer large files to and from the server.

## Intended usage:

### Compiling and running
-'make compile' for both ./server and ./client

### Client commands
- `NAME <username>`: (set username)
- `MSG <text...>`: (send to current room)
- `WHO`: (list users)
- `QUIT`: (disconnect with sighandler)



## Technical Details:
### Key systems concepts used
1. **Sockets:** Client/server communication
2. **Finding Info about files:** Get bytes to recv for file transfer, exist or not
3. **Signals:** Server handles `SIGINT`/`SIGTERM` for graceful shutdown, notify clients, close sockets, flush logs, avoid zombie processes with `SIGCHLD`/`waitpid`.
4. **Working With Files:**: Log files, file transfer opening and reading files into buffer

### Server architecture
We will implement:
- **Select Server design:**
  - Parent accepts connections and manages global rooms and connected clients
  - The parent collects Client_ids, reading the only readble listen socket
  - Parent broadcasts outgoing messages to all clients
- (And optionally):
  - server commands: kick, announcement
  - server broadcasting to client private


### MVP:
Client → Server:
- `NAME <username>`
- `MSG <text...>`
- `WHO`: lists people
- `QUIT`

Server → Client:
- sends recv() data to other clients
Optionally:
- `MSG <text>`: psa
- `MSG <username> <text...>`: (server messaging client privtely)
- `ROOM <room> MSG <username> <text...>`

### Responsibilities
- **Clayton Zhu:**
  - socket setup, accept loop, file transfer, parsing commands, client send and recieve
- **Ivan Chen:**
  - research ncurses, display formatting, reconnect behavior
- **Patrick Tang:**
  - select server, parsing commands, formatting
- **Shared:**
  - testing, README

### Stretch goals (only after MVP is stable)
- Non-Global Private rooms
- Persistent chat logs with rotation
- Rate limiting / spam protection
- Server-side moderation commands (kick/mute/psa/dm)

## Intended pacing 

**Day 1–2**
- Repo setup (Makefile, basic structure), documentation
- Basic server/client connect, `NAME`, `MSG`, `QUIT`
- With simple output

**Day 3–4**
- `WHO` and join/leave notices 
- Stress test with multiple clients
- Start ncurses implementation, silently rewriting prev code 

**Day 5–6** 
- Improved parsing and error handling
- implement ncurses

**Day 7–8**
- Handle disk path
- Stat metadata

**Day 9**
- Signals + graceful shutdown; zombie prevention
- handle bugs, disconnects, malformed inputs, file edge cases

**Day 10**
- Polish README, demo steps, known limitations
- Presentation prep and final testing
