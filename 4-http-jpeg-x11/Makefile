LDFLAGS = -lX11 -ljpeg
CFLAGS = -Wall
#ARGS=http://kartolo.sby.datautama.net.id/Centos/6/os/x86_64/isolinux/splash.jpg
ARGS=http://kartolo.sby.datautama.net.id/debian/README.mirrors.txt

main: main.o

clean:
	rm main

watch_%:
	nodemon --delay 0.5 -w $*.c -x '$(CC) $(CFLAGS) -o $* $*.c && ./$* $(ARGS)'

watch: watch_main

.PHONY: clean all watch watch_%
