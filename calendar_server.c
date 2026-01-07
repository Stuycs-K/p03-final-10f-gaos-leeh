#include "server_behavior.h"
#include "networking.h"

int main() {
    signal(SIGINT, sighandler);
    
    int listen_socket = server_setup();
    
    printf("Calendar Server started on port %s\n", PORT);
    printf("Waiting for client connections...\n");
    
    while (1) {
        int client_socket = server_tcp_handshake(listen_socket);
        
        int f = fork();
        
        if (f == 0) {
            close(listen_socket);
            handle_client(client_socket);
            close(client_socket);
            exit(0);
        }
        else {
            close(client_socket);
        }
    }
    
    close(listen_socket);
    return 0;
}