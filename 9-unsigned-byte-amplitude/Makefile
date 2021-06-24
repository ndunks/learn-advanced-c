CFLAGS = -lasound -lm
all: main

clean:
	rm main

watch:
	nodemon --delay 0.5 -w main.c -x '$(CC) $(CFLAGS) -o main main.c && ./main'

.PHONY: clean all watch