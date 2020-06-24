CFLAGS = -lpthread
main:

clean:
	rm main

watch:
	nodemon --delay 0.5 -w main.c -x '$(CC) $(CFLAGS) -o main main.c && ./main $(ARGS)'

.PHONY: clean all watch