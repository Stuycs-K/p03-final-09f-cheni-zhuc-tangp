# Final Project Proposal

# Group Members:
- Clayton Zhu
- Patrick Tang
- Ivan Chen 

## Intentions:
We will build a terminal-based, multi-client chat system with a central server. Multiple clients can connect concurrently, choose usernames, and send messages that are broadcast to everyone.
(and optionally, extend such that user can provide paths on the local disk and/or smaller, more private rooms)

## Intended usage:

### Compiling and running
-'make compile' for both ./server and ./client

### Client commands
- `NAME <username>`: (set username)
- `MSG <text...>`: (send to current room)
- `WHO`: (list users)
- `QUIT`: (disconnect with sighandler)

Extra room commands:
- `JOIN <room>`: where <room> is WKP? 
- `LEAVE`: Leave

## Technical Details:
### Key systems concepts used
1. **Sockets:** Client/server communication 
2. **Processes (`fork()`)):** Server forks a child process to handle each connected client
3. **Pipes:** Client-handler processes communicate with a parent “router” process using pipes so messages can be broadcast across all clients and rooms.
4. **Files + `stat()`:** Do something with a given path to a file stored on disk. stat() for metadata and bytes
5. **Signals:** Server handles `SIGINT`/`SIGTERM` for graceful shutdown, notify clients, close sockets, flush logs, avoid zombie processes with `SIGCHLD`/`waitpid`.


### Server architecture
We will implement either:
- **Fork-per-client design:**
  - Parent accepts connections and manages global rooms and connected clients
  - For each client, parent forks a handler process.
  - Each handler reads client commands and forwards to the parent over a pipe.
  - Parent broadcasts outgoing messages to all relevant handlers (also over pipes).
- Message routing supports:
  - global broadcast
  - server announcements (join/leave, errors)?


### MVP:
Client → Server:
- `NAME <username>`
- `MSG <text...>`
- `WHO`
- `QUIT`
Optional:
- `JOIN <room>`
- `LEAVE`

Server → Client:
- `WELCOME <client_id>`
- `OK <message>`
- `ERR <reason>`
- `MSG <username> <text...>`: (admin messaging client)
- `JOIN/LEAVE/SHUTDOWN'
Optional:
- `ROOM <room> MSG <username> <text...>`

### Responsibilities
- **Clayton Zhu:**
  - socket setup, accept loop, fork model, pipe setup, client lifecycle
- **Ivan Chen:**
  - client, command parsing, display formatting, reconnect behavior
- **Patrick Tang:**
  - `stat()` formatting, graceful shutdown, and leftover tasks directed by other partners
- **Shared:**
  - testing, README

### Stretch goals (only after MVP is stable)
- Rooms with access control and `/invite`
- Persistent chat logs with rotation
- Rate limiting / spam protection
- Server-side moderation commands (kick/mute)

## Intended pacing 

**Day 1–2**
- Repo setup (Makefile, basic structure), documentation
- Basic server/client connect, `NAME`, `MSG`, `QUIT`

**Day 3–4**
- Concurrency model completed fork + pipes
- `WHO` and join/leave notices 
- Stress test with multiple clients 

**Day 5–6** 
- Improved parsing and error handling

**Day 7–8**
- Handle disk path
- Stat metadata

**Day 9**
- Signals + graceful shutdown; zombie prevention
- handle bugs, disconnects, malformed inputs, file edge cases

**Day 10**
- Polish README, demo steps, known limitations
- Presentation prep and final testing
