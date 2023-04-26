CC=clang
LLIBS=
CFLAGS=-pipe -O2 -g -flto -Wall -Wextra -fPIC -std=gnu17

OBJS= \
	build.o \
	read_cfg.o

default: all

all: main

main: $(OBJS)
	$(CC) $(CFLAGS) -o ../build_fedora.elf $(OBJS) $(LLIBS)
