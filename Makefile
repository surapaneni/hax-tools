CC=	gcc

CFLAGS=	-std=c99 -Wall -Wextra -O2 -pipe -D_BSD_SOURCE
LDFLAGS=

fastest_mirror_CFLAGS=	$(CFLAGS) -D_POSIX_C_SOURCE=200809L
fastest_mirror_LDFLAGS=	$(LDFLAGS) -lrt

tools:	revdns fastest_mirror 

revdns:	revdns.c
	$(CC) $(CFLAGS) revdns.c -o $@

fastest_mirror:	fastest_mirror.c hostlist.o
	$(CC) $(fastest_mirror_CFLAGS) $(fastest_mirror_LDFLAGS) $(OS_FLAGS) hostlist.o fastest_mirror.c -o $@

hostlist.o:	hostlist.c hostlist.h
	$(CC) $(CFLAGS) -c hostlist.c

clean:
	rm -f revdns fastest_mirror hostlist.o
