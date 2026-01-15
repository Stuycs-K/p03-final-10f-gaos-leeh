# Dev Log:

This document must be updated daily every time you finish a work session.

## Hyunseo "Eileen Lee"

### 2026-01-06 - Brief description
Pushed the Proposal Doc into the repo, further allocated tasks following the timeline
On server_behavior.c, added handle logic (not fully implemented), singhandler, fixed some dependency/h fils issues across files (as of 6:30 am)


### 2026-01-07 - TBA
Fixed some linking issues in the makefile (as of 8:48am)
tried to fix networking socket issues in networking.c and some data type in server behavior.c, but failed (worked from: 11pm - this morning)

### 2026-01-08 - Server Connection
Got the calendar server and calendar client to connect, using select. (as of 1 am)

### 2026-01-09 - Handling inputs, formatting in terminal settings
got the process_command and handle_create_event functions working (as of the end of class on Jan 9)

### 2026-01-11 - Ncurses basic setup/color
Ncurses trial on calender_ui.c

### 2026-01-12 - updated monthly view
now supports "MONTH mm/yyyy" command on string based terminal, still needs to work out visuals/ncurses

### 2026-01-13 - ncurses preliminary implementation
terminal resizing, made the TUI be pitch black, and now supports keybind to naviage thru inputs like up and down arrows. still need to figure out how to display calendar though.

<<<<<<< HEAD
### 2026-01-14 - menu instruciton null termination, ncurses display and arrow keybind
included ncurses.o and overridden same named function with text.c and ncurses.c, null terminated string printed for the client so there's no more "here" garbage pointing.

### 2025-01-15 - syntax instruction on the client menu
=======
### 2026-01-14 -
reverted the commit that nullified all ncurses functions, found isseus like server stacking all responses even after deleting, and not printing the calendar text
>>>>>>> f8cd5996e7606346762dd8a1e4c034848bc27c0a
