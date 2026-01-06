.PHONY: calendar clean
calendar: compile
	@./calendar
compile: calendar.o
	gcc -o calendar calendar.o
calendar.o: calendar.c calendar.h
	gcc -c calendar.c
clean:
	rm -f *.o
