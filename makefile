.PHONY: calendar_setup open_calendar compile clean
calendar_setup: compile
	@./calendar_server
open_calendar: compile
	@./calendar_client
compile: calendar_server.o server_behavior.o calendar_client.o client_behavior.o networking.o
	@gcc -o calendar_server calendar_server.o server_behavior.o
	@gcc -o calendar_client calendar_client.o client_behavior.o
server_behavior.o: server_behavior.c networking.h
	@gcc -Wall server_behavior.c
client_behavior.o: client_behavior.c networking.h
	@gcc -Wall client_behavior.c
calendar_server.o: calendar_server.c server_behavior.h
	@gcc -Wall calendar_server.c
calendar_client.o: calendar_client.c client_behavior.h
	@gcc -Wall calendar_client.c
networking.o: networking.c
	@gcc -Wall networking.c
clean:
	rm -f *.o calendar_server calendar_client
