CFLAGS   := -pthread
PROGRAMS := $(patsubst %.c, %, $(wildcard *.c))

help:
	$(foreach P, $(PROGRAMS), $(info make $(P)))
	$(foreach P, $(PROGRAMS), $(info make watch_$(P)))
	$(info make clean)
	@#

$(PROGRAMS): % : %.c
	$(CC) $(CFLAGS) -o $* $*.c

clean:
	rm $(PROGRAMS)

watch_%:
	nodemon --delay 0.5 -w $*.c -x 'make $* && ./$* $(ARGS)'

.PHONY: help clean all watch_%