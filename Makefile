CC=	gcc

CFLAGS=	-std=c99 -Wall -Wextra -O2 -pipe
LDFLAGS=

soc_times_CFLAGS=	$(CFLAGS) -D_POSIX_C_SOURCE=200112L
soc_times_LDFLAGS=	$(LDFLAGS) -lrt

tools:	revdns soc_times

revdns:	revdns.c
	$(CC) $(CFLAGS) revdns.c -o $@

soc_times:	soc_times.c
	$(CC) $(soc_times_CFLAGS) $(soc_times_LDFLAGS) soc_times.c -o $@

clean:
	rm -f revdns soc_times
