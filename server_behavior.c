#include "networking.h"
#include "server_behavior.h"

struct DateNode *calendar_head = NULL;
int next_event_id = 1;
static int client_counter = 0;

void sighandler(int signo) {
    if (signo == SIGINT) {
        printf("\nShutting down server...\n");
        save_calendar();
        exit(0);
    }
}

int get_client_id() {
    return ++client_counter;
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

void save_calendar() {
    FILE *fp = fopen("calendar_data.txt", "w");
    if (!fp) return;

    struct DateNode *date = calendar_head;
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

void load_calendar() {
    FILE *fp = fopen("calendar_data.txt", "r");
    if (!fp) return;

    // Loading logic here

    fclose(fp);
}
