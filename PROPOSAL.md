# Final Project Proposal

## Group Members:

Sandra Gao, Hyunseo "Eileen" Lee

# Intentions:

When coordinating an event or a schedule with multiple people (especially with different sub-groups), it is difficult for everyone to agree on a time window. In order to solve this, we aim to make a shared calendar space where you can indicate your availabilites (both private and public), see others' events, and delegate tasks/events for certain people via invite and event sharing.

# Intended usage:

Features We're Aiming by MVP Deadline (12 Jan 2026)

- Supports dates in 2026
- Interface switching between monthly views (e.g. to move between times, the calender will by default switch from Jan → Feb)
- Creating events:
- Can select it to be all-day-long event, or blocking out designated time block
- Can choose it to be public (visible to everyone) or private
- Switch between public and private: invitation to certain clients allowed
- Events have location, description, guest slots for allocation

Extra Features
- Time zones in GMT
- Weekly/daily views
- Group making features (three layers of event visibility: only to me, only to group, and to everyone)
- Select/deselect group events from display
- Separate chatroom(s) to better accomodate event discussions and plannings

Interface description

Main Calendar View:
- Monthly grid display showing all days of 2026
- Navigation controls to switch between months (January → February → December)
- Color-coded event blocks distinguishing between: private events (visible only to the user), public events (visible to all users), invited events (shared with specific users)

Event Creation Interface:
- Modal dialog or side panel activated by clicking on a date/time slot
- Form fields including: event title and description, date selector with all-day toggle, time block selector (start/end times) for non-all-day events, location, privacy setting dropdown, guest allocation slots

Event Management:
- Click on existing events to view details
- Edit/delete options for owned events
- Accept/decline invitations for invited events
- Toggle between viewing public events only or all accessible events

Navigation:
- Month selector dropdown or arrow navigation
- "Today" button to return to current date
- Filter controls for event visibility preferences

# Technical Details:

Linked lists:
- event storage, guest lists, user directory, and sorted event lists
Dynamic memory allocation
- event nodes, user nodes, and string data like titles, descriptions, locations.
- navigation throughout linked lists, for updates and deletions on events
- variable string allocation in case of edits

Structs:
- event structs: title, date, time, location, privacy, guest list pointer
- user structs: containing user information like ID, name, pointer to next user
- date/time structs: calendar dates and time blocks

File I/O:
- fopen, fread, fwrite, fclose: Save calendar data to files and load on startup
- text file parsing for events

Sandra's Responsibilities:
- Frontend user interface development and integration using html/css/javascript

Eileen Responsibilities:
- event functions: createevent, deleteevent, updateevent, findevent
- master calender linked list
- privacy and permission checking logic

Shared:
- design and implement event and user structs
- create master calendar linked list (date → events structure)
- linked list operations
- error handling
- formatting


# Intended pacing:

08 Jan 2026: server running ; clients able to connect

10 Jan 2026: interface ; creating public / private events

12 Jan 2026: MVP
