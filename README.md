[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/QfAIDTGi)

# Shared Calendar

### Kepler's Law of Planetary Motion

Hyunseo "Eileen" Lee ; Sandra Gao

### Project Description:

When coordinating an event or a schedule with multiple people (especially with different sub-groups), it is difficult for everyone to agree on a time window. In order to solve this, we aim to make a shared calendar space where you can indicate your availabilites (both private and public), see others' events, and delegate tasks/events for certain people via invite and event sharing.

link to the video: https://drive.google.com/drive/folders/1KUdrKaTGvbNjclmNGwQLBp0ZC7vNzBSe?usp=sharing

### Instructions:

If ncurses isn't already installed, the user needs to download it prior to the usage.
There should be a server hosting all the sockets, so someone should have run the "make compile calendar_setup" command in the terminal.
Afterwards, anyone can run "make compile open_calendar" to log on to the shared calendar.

### Navigation
- **Left/Right Arrow Keys** - Navigate between months
- **Up/Down Arrow Keys** - Jump 3 months back/forward
- **Up/Down (in command prompt)** - Navigate command history

### Available Commands

Type `HELP` at any time to see the command reference.

#### CREATE - Create a new event
CREATE name|description|permissions|all_day|month|day|year
- `permissions`: `0` = public (visible to all), `-1` = private (only you can see)
- `all_day`: `1` = all-day event, `0` = timed event

For timed events, add time information:
```
CREATE Concert|Laufey|0|0|1|23|2026|20|00|23|30
```
(Format: `|start_hour|start_min|end_hour|end_min`)

#### VIEW - View events on a specific date
```
VIEW month|day|year
```

#### MONTH - View all events in a month
```
MONTH month|year
```

**Example:**
```
MONTH 1|2026
```

#### DELETE - Delete an event (must be owner)
```
DELETE event_id
```

**Example:**
```
DELETE 1
```

#### Other Commands
- `HELP` - Display command reference
- `quit` or `exit` - Exit the calendar

### Implemented Features
- Client-server architecture with `select()` for concurrent connections
- Shared memory for calendar data across all clients
- Real-time event synchronization between clients
- Public and private event visibility settings
- Monthly calendar navigation with arrow keys
- Command history navigation (up/down arrows in command prompt)
- Event creation, viewing, and deletion
- Permission-based event management (only owners can delete their events)
- Interactive ncurses-based terminal UI
- Today's date highlighting
- Persistent calendar data storage

### Resources/ References:

https://curtsinger.cs.grinnell.edu/teaching/2025S/CSC161/readings/ncurses/
