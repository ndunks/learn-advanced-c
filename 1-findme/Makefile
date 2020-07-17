
ifdef DEBUG
CFLAGS := -g
else
$(info Build optimized)
# -Os = Optimize code size
# -s  = Strip symbol table
CFLAGS := -Os -s
endif

all: findme

clean:
	rm findme || false

.PHONY: clean all
