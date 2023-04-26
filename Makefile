CC=clang
CFLAGS=-pipe -O2 -g -flto -Wall -Wextra -fPIC -std=gnu17

default:
	$(CC) build.c $(CFLAGS) -o ../build_fedora.elf
	strip build_fedora.elf
