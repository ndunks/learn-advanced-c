CFLAGS = -lpthread
all: server client

clean:
	rm server client

watch_%:
	nodemon --delay 0.5 -w $*.c -x '$(CC) $(CFLAGS) -o $* $*.c && ./$* $(ARGS)'

.PHONY: clean all watch_%