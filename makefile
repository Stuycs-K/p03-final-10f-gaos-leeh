.PHONY: calendar clean
calendar_setup: compile
	@./calendar_server
open_calendar: compile
	@./calendar_client
compile: calendar_server.o calendar_client.o
	@gcc -o calendar_server calendar_server.o
	@gcc -o calendar_client calendar_client.o
calendar_server.o: calendar_client.c calendar.h
	@gcc -c calendar_server.o
calendar_client.o: calendar_client.c calendar.h
	@gcc -c calendar_client.c
clean:
	rm -f *.o
