# Dev Log:

This document must be updated daily every time you finish a work session.

## Patrick Tang:

### 2026-01-06 - Added Username Capability
Upon connecting with the server as a client, you will be prompted to enter a username. Once given, you will use that username when you type messages into the server.(~30 minutes)

### 2026-01-07 - Select and Client Command
Changed logic to work with select taught in class(~45 minutes)
Started to handle client commands in the server(~30 minutes)

### 2026-01-08 - Name and Quit
Changed how usernames are handled and added quitting capabilities(~45 minutes)

### 2026-01-09 - 2026-01-011  - Rework select and client commands
Reworked server.c in how client commands are handled with select(~1.5 hours)
Reworked client.c to read stdin, send, and receive data to the server(~1 hour)

### 2026-01-12 - Added WHO and Fixed Quit
Rewrote client commands to use / for special cases(~5 minutes)
Wrote WHO to display active users(~45 minutes)
Fixed QUIT to correctly quit(~5 minutes)

### 2026-01-13 - Added Sighandler and Fixed bugs
Fixed issue with newline for client names(~10 minutes)
Wrote Sighandler to handle graceful shutdowns(~30 minutes)
