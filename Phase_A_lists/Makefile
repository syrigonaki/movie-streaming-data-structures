CC=gcc
CFLAGS=-ansi -g

cs240StreamingService: main.c users.c movies.c other.c streaming_service.h
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean

clean:
	rm -f cs240StreamingService
