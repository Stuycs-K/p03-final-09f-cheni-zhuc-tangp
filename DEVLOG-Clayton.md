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