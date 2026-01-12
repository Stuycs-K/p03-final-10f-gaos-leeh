.PHONY: calendar_setup open_calendar compile clean
calendar_setup: compile
	@./calendar_server
open_calendar: compile
	@./calendar_client
compile: calendar_server.o server_behavior.o calendar_client.o client_behavior.o networking.o text.o
	@gcc -o calendar_server calendar_server.o server_behavior.o networking.o text.o
	@gcc -o calendar_client calendar_client.o client_behavior.o networking.o text.o
server_behavior.o: server_behavior.c networking.h text.h
	@gcc -c -Wall server_behavior.c
client_behavior.o: client_behavior.c networking.h text.h
	@gcc -c -Wall client_behavior.c
calendar_server.o: calendar_server.c server_behavior.h
	@gcc -c -Wall calendar_server.c
calendar_client.o: calendar_client.c client_behavior.h
	@gcc -c -Wall calendar_client.c
networking.o: networking.c
	@gcc -c -Wall networking.c
text.o: text.c
	@gcc -c -Wall text.c
clean:
	rm -f *.o calendar_server calendar_client
