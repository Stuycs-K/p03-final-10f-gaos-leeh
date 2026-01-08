#include "server_behavior.h"

int main() {
    key_t key = ftok("calendar_server.c", 65);
    shmid = shmget(key, sizeof(struct SharedCalendar), 0666 | IPC_CREAT);
    if (shmid < 0) {
        perror("shmget failed");
        exit(1);
    }

    shared_calendar = (struct SharedCalendar *)shmat(shmid, NULL, 0);
    if (shared_calendar == (void *)-1) {
        perror("shmat failed");
        exit(1);
    }

    shared_calendar->calendar_head = NULL;
    shared_calendar->next_event_id = 1;
    shared_calendar->client_counter = 0;

    // load_calendar(shared_calendar);

    signal(SIGINT, sighandler);

    int listen_socket = server_setup();

    printf("Calendar Server started on port %s\n", PORT);
    printf("Waiting for client connections...\n");

    int client_sockets[MAX_CLIENTS];
    int client_ids[MAX_CLIENTS];
    for (int i = 0; i < MAX_CLIENTS; i++) {
        client_sockets[i] = -1;
        client_ids[i] = -1;
    }

    fd_set read_fds;
    int max_fd = listen_socket;

    while (1) {

        FD_ZERO(&read_fds);
        FD_SET(listen_socket, &read_fds);

        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (client_sockets[i] > 0) {
                FD_SET(client_sockets[i], &read_fds);
                if (client_sockets[i] > max_fd) {
                    max_fd = client_sockets[i];
                }
            }
        }

        int activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);

        if (activity < 0) {
            perror("select error");
            continue;
        }

        if (FD_ISSET(listen_socket, &read_fds)) {
            int new_socket = server_tcp_handshake(listen_socket);

            int slot = -1;
            for (int i = 0; i < MAX_CLIENTS; i++) {
                if (client_sockets[i] == -1) {
                    slot = i;
                    break;
                }
            }

            if (slot == -1) {
                printf("Max clients reached. Rejecting connection.\n");
                char *msg = "ERROR: Server full\n";
                write(new_socket, msg, strlen(msg));
                close(new_socket);
            } else {
                client_sockets[slot] = new_socket;
                client_ids[slot] = ++shared_calendar->client_counter;

                char buffer[BUFFER_SIZE];
                sprintf(buffer, "Connected to Calendar Server. Your ID: %d\n", client_ids[slot]);
                write(new_socket, buffer, strlen(buffer));

                printf("New client connected: ID %d, socket %d\n", client_ids[slot], new_socket);
            }
        }

        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (client_sockets[i] > 0 && FD_ISSET(client_sockets[i], &read_fds)) {
                char buffer[BUFFER_SIZE];
                memset(buffer, 0, BUFFER_SIZE);

                int bytes_read = read(client_sockets[i], buffer, BUFFER_SIZE - 1);

                if (bytes_read <= 0) {
                    printf("Client %d disconnected\n", client_ids[i]);
                    close(client_sockets[i]);
                    client_sockets[i] = -1;
                    client_ids[i] = -1;
                } else {
                    // process_command(buffer, client_sockets[i], client_ids[i], shared_calendar);
                }
            }
        }
    }

    close(listen_socket);
    shmdt(shared_calendar);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
