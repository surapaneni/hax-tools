CC=	gcc

CFLAGS=	-std=c99 -Wall -Wextra -O2 -pipe
LDFLAGS=

soc_times_CFLAGS=	$(CFLAGS) -D_POSIX_C_SOURCE=200112L
soc_times_LDFLAGS=	$(LDFLAGS) -lrt

tools:	revdns soc_times

revdns:	revdns.c
	$(CC) $(CFLAGS) revdns.c -o $@

soc_times:	soc_times.c hostlist.o
	$(CC) $(soc_times_CFLAGS) $(soc_times_LDFLAGS) hostlist.o soc_times.c -o $@

hostlist.o:	hostlist.c hostlist.h
	$(CC) $(CFLAGS) -c hostlist.c

clean:
	rm -f revdns soc_times hostlist.o
