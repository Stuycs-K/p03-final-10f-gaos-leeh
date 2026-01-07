.PHONY: calendar_setup open_calendar compile clean
calendar_setup: compile
	@./calendar_server
open_calendar: compile
	@./calendar_client
compile: calendar_server.o server_behavior.o calendar_client.o client_behavior.o
	@gcc -o calendar_server calendar_server.o server_behavior.o
	@gcc -o calendar_client calendar_client.o client_behavior.o
server_behavior.o: server_behavior.c
	@gcc -c server_behavior.c
client_behavior.o: client_behavior.c
	@gcc -c client_behavior.c
calendar_server.o: calendar_client.c server_behavior.h
	@gcc -c calendar_server.o
calendar_client.o: calendar_client.c client_behavior.h
	@gcc -c calendar_client.c
clean:
	rm -f *.o
