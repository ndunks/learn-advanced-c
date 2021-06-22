CFLAGS = -lasound
all: main

clean:
	rm main

watch_%:
	nodemon --delay 0.5 -w $*.c -x '$(CC) $(CFLAGS) -o $* $*.c && ./$*'

test: main
	./main > record.raw
	aplay -c 1 -f S16_LE -r 2000 record.raw

.PHONY: clean all test watch_%