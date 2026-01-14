#include "networking.h"
#include "server_behavior.h"

struct SharedCalendar *shared_calendar = NULL;
int shmid;

void sighandler(int signo) {
    if (signo == SIGINT) {
        printf("\nShutting down server...\n");
        save_calendar(shared_calendar);

        shmdt(shared_calendar);
        shmctl(shmid, IPC_RMID, NULL);

        exit(0);
        exit(0);
    }
}

struct DateNode* find_or_create_date(int month, int day, int year, struct SharedCalendar *shared_cal) {
    struct DateNode *curr = shared_cal->calendar_head;
    struct DateNode *prev = NULL;

    while (curr) {
        if (curr->year == year && curr->month == month && curr->day == day) {
            return curr;
        }
        prev = curr;
        curr = curr->next;
    }

    struct DateNode *new_date = malloc(sizeof(struct DateNode));
    new_date->month = month;
    new_date->day = day;
    new_date->year = year;
    new_date->events = NULL;
    new_date->next = NULL;

    if (shared_cal->calendar_head == NULL) {
        shared_cal->calendar_head = new_date;
    } else {
        prev->next = new_date;
    }

    return new_date;
}

void handle_client_message(int client_socket, int client_id, struct SharedCalendar *shared_cal) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);

    int bytes_read = read(client_socket, buffer, BUFFER_SIZE - 1);

    if (bytes_read <= 0) {
        printf("Client %d disconnected\n", client_id);
        close(client_socket);
        return;
    }

    process_command(buffer, client_socket, client_id, shared_cal);
}

void handle_view_events(char *buffer, int socket, int client_id, struct SharedCalendar *shared_cal) {
    int month, day, year;
    sscanf(buffer, "VIEW %d|%d|%d", &month, &day, &year);

    struct DateNode *date = shared_cal->calendar_head;
    while (date) {
        if (date->year == year && date->month == month && date->day == day) {
            break;
        }
        date = date->next;
    }

    char response[BUFFER_SIZE * 4];
    memset(response, 0, sizeof(response));

    if (!date || !date->events) {
        sprintf(response, "No events on %d/%d/%d\n", month, day, year);
        write(socket, response, strlen(response));
        return;
    }

    sprintf(response, "Events on %d/%d/%d:\n", month, day, year);

    struct EventNode *event = date->events;
    while (event) {
        if (event->permissions == 0 || event->owner_id == client_id) {
            char event_str[512];
            if (event->all_day) {
                sprintf(event_str, "[ID:%d] %s (All Day) - %s\n",
                        event->event_id, event->name, event->description);
            } else {
                sprintf(event_str, "[ID:%d] %s (%02d:%02d - %02d:%02d) - %s\n",
                        event->event_id, event->name,
                        event->start_hour, event->start_minute,
                        event->end_hour, event->end_minute,
                        event->description);
            }
            strcat(response, event_str);
        }
        event = event->next;
    }

    write(socket, response, strlen(response));
}

void handle_month_view(char *buffer, int socket, int client_id, struct SharedCalendar *shared_cal) {
    int month, year;
    sscanf(buffer, "MONTH %d|%d", &month, &year);

    char response[BUFFER_SIZE * 10];
    memset(response, 0, sizeof(response));
    sprintf(response, "Events in %d/%d:\n", month, year);

    struct DateNode *date = shared_cal->calendar_head;
    int found = 0;

    while (date) {
        if (date->year == year && date->month == month) {
            struct EventNode *event = date->events;
            while (event) {
                if (event->permissions == 0 || event->owner_id == client_id) {
                    char event_str[512];
                    sprintf(event_str, "%d/%d: [ID:%d] %s\n",
                            date->month, date->day, event->event_id, event->name);
                    strcat(response, event_str);
                    found = 1;
                }
                event = event->next;
            }
        }
        date = date->next;
    }

    if (!found) {
        sprintf(response, "No events in %d/%d\n", month, year);
    }

    write(socket, response, strlen(response));
}


void handle_delete_event(char *buffer, int socket, int client_id, struct SharedCalendar *shared_cal) {
    int event_id;
    sscanf(buffer, "DELETE %d", &event_id);

    struct DateNode *date = shared_cal->calendar_head;
    while (date) {
        struct EventNode *event = date->events;
        struct EventNode *prev = NULL;

        while (event) {
            if (event->event_id == event_id) {
                if (event->owner_id != client_id) {
                    write(socket, "ERROR: Permission denied\n", 25);
                    return;
                }

                if (prev == NULL) {
                    date->events = event->next;
                } else {
                    prev->next = event->next;
                }

                free(event);
                save_calendar(shared_cal);
                write(socket, "SUCCESS: Event deleted\n", 23);
                return;
            }
            prev = event;
            event = event->next;
        }
        date = date->next;
    }

    write(socket, "ERROR: Event not found\n", 23);
}

void handle_create_event(char *buffer, int socket, int client_id, struct SharedCalendar *shared_cal) {
    char name[128], description[256];
    int permissions, all_day, month, day, year;
    int start_hour = 0, start_min = 0, end_hour = 0, end_min = 0;

    char *buf_ptr = buffer;
    char *token;

    token = strsep(&buf_ptr, " ");

    token = strsep(&buf_ptr, "|");
    if (!token) { write(socket, "ERROR: Invalid format\n", 22); return; }
    strcpy(name, token);

    token = strsep(&buf_ptr, "|");
    if (!token) { write(socket, "ERROR: Invalid format\n", 22); return; }
    strcpy(description, token);

    token = strsep(&buf_ptr, "|");
    if (!token) { write(socket, "ERROR: Invalid format\n", 22); return; }
    permissions = atoi(token);

    token = strsep(&buf_ptr, "|");
    if (!token) { write(socket, "ERROR: Invalid format\n", 22); return; }
    all_day = atoi(token);

    token = strsep(&buf_ptr, "|");
    if (!token) { write(socket, "ERROR: Invalid format\n", 22); return; }
    month = atoi(token);

    token = strsep(&buf_ptr, "|");
    if (!token) { write(socket, "ERROR: Invalid format\n", 22); return; }
    day = atoi(token);

    token = strsep(&buf_ptr, "|\n");
    if (!token) { write(socket, "ERROR: Invalid format\n", 22); return; }
    year = atoi(token);

    if (!all_day) {
        token = strsep(&buf_ptr, "|");
        if (token) start_hour = atoi(token);

        token = strsep(&buf_ptr, "|");
        if (token) start_min = atoi(token);

        token = strsep(&buf_ptr, "|");
        if (token) end_hour = atoi(token);

        token = strsep(&buf_ptr, "|\n");
        if (token) end_min = atoi(token);
    }

    struct EventNode *new_event = malloc(sizeof(struct EventNode));
    new_event->event_id = shared_cal->next_event_id++;
    strcpy(new_event->name, name);
    strcpy(new_event->description, description);
    new_event->owner_id = client_id;
    new_event->permissions = permissions;
    new_event->all_day = all_day;
    new_event->start_hour = start_hour;
    new_event->start_minute = start_min;
    new_event->end_hour = end_hour;
    new_event->end_minute = end_min;
    new_event->next = NULL;

    struct DateNode *date = find_or_create_date(month, day, year, shared_cal);
    new_event->next = date->events;
    date->events = new_event;

    save_calendar(shared_cal);

    char response[BUFFER_SIZE];
    sprintf(response, "SUCCESS: Event created with ID %d\n", new_event->event_id);
    write(socket, response, strlen(response));
}

void process_command(char *buffer, int socket, int client_id, struct SharedCalendar *shared_cal) {
    char command[32];
    sscanf(buffer, "%s", command);

    if (strcmp(command, "CREATE") == 0) {
        handle_create_event(buffer, socket, client_id, shared_cal);
    }
    else if (strcmp(command, "VIEW") == 0) {
        handle_view_events(buffer, socket, client_id, shared_cal);
    }
    else if (strcmp(command, "DELETE") == 0) {
        handle_delete_event(buffer, socket, client_id, shared_cal);
    } else if (strcmp(command, "MONTH") == 0) {
        handle_month_view(buffer, socket, client_id, shared_cal);
    } else {
        write(socket, "ERROR: Unknown command\n", 23);
    }
}

void save_calendar(struct SharedCalendar *shared_cal) {
    FILE *fp = fopen("calendar_data.txt", "w");
    if (!fp) return;

    struct DateNode *date = shared_cal->calendar_head;
    while (date) {
        struct EventNode *event = date->events;
        while (event) {
            fprintf(fp, "%d|%d|%d|%d\n",
                    event->event_id, date->month, date->day, date->year);

            fprintf(fp, "%s|%s|%d|%d|%d\n",
                    event->name, event->description, event->owner_id,
                    event->permissions, event->all_day);

            fprintf(fp, "%d|%d|%d|%d\n",
                    event->start_hour, event->start_minute,
                    event->end_hour, event->end_minute);

            event = event->next;
        }
        date = date->next;
    }

    fclose(fp);
}



// reminder for eileen to do this tonight (13/01/2026)
/*
void load_calendar() {
    FILE *fp = fopen("calendar_data.txt", "r");
    if (!fp) return;

    // Loading logic here
    fclose(fp);
}
    */
