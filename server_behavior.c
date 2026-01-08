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
    
    // process_command(buffer, client_socket, client_id, shared_cal);
}

/* void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    int client_id = get_client_id();

    sprintf(buffer, "Connected to Calendar Server. Your ID: %d\n", client_id);
    write(client_socket, buffer, strlen(buffer));

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_read = read(client_socket, buffer, BUFFER_SIZE - 1);

        if (bytes_read <= 0) {
            printf("Client %d disconnected\n", client_id);
            break;
        }

        process_command(buffer, client_socket, client_id);
    }
}


void process_command(char *buffer, int socket, int client_id) {
    char command[32];
    sscanf(buffer, "%s", command);

    if (strcmp(command, "CREATE") == 0) {
        handle_create_event(buffer, socket, client_id);
    }
    else if (strcmp(command, "VIEW") == 0) {
        handle_view_events(buffer, socket, client_id);
    }
    else if (strcmp(command, "DELETE") == 0) {
        handle_delete_event(buffer, socket, client_id);
    }
    else if (strcmp(command, "MONTH") == 0) {
        handle_month_view(buffer, socket, client_id);
    }
    else {
        write(socket, "ERROR: Unknown command\n", 23);
    }
} */

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


/*
void load_calendar() {
    FILE *fp = fopen("calendar_data.txt", "r");
    if (!fp) return;

    // Loading logic here

    fclose(fp);
}
    */

