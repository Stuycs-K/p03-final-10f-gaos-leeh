# Dev Log:

This document must be updated daily every time you finish a work session.

## Sandra Gao

### 2026-01-06 - update documentation

update README.md : names [2]
update PROPOSAL.md : update intended pacing [2]
setup : create initial .h .c makefiles [5]
structs : event and date structs for calendar [2]
first methods : display_calendar() create_event() [40]

### 2026-01-07 - debugging

fix undefined reference : error when compiling kept popping back up after I thought I fixed it [45]

## 2026-01-08 - ui

print_frame() : start method to print calendar frame [90]

## 2026-01-09 - ui continued

repeat() : helper function to print tops / bottoms of calendar frame [5]
print_frame() : seems to be working ; move to testing [20]
display_calendar() : print_frame() ; print current month, week header, dates [60]

## 2026-01-10 - text.h library

text.c : migrate printing escape sequence methods from APCS terminal game ; update other libraries, makefile to adjust [30]
terminal size : define constants for terminal width / height
print_prompt() : make client looping more compact [10]

## 2026-01-12 - numbering the calendar

finding weekday : find the weekday for the first of the month to print the rest of the month [75]
time.h : implementing library for struct tm
formatting : debugging overlapping prints ; month overwritten, prompt overwrites frame [10]

## 2026-01-13 - reorganize

calendar.h : move event, date structs to new library ; adjust makefile [3]
shift : implement view months other than the current [30]

## 2026-01-14 - display months

conway : attempt to calculate first of months other than current month using conway's algorithm [90]
display_month() : use ncurses to modify printing

## 2026-01-15 - reorganize

ui.h : rename ncurses
displya_month() : adjust for ncurses methods
