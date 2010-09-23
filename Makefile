CC=	gcc
CFLAGS=	-std=c99 -Wall -Wextra -O2 -pipe

tools:	revdns soc_times

revdns:	revdns.c
	$(CC) $(CFLAGS) revdns.c -o $@

soc_times:	soc_times.c
	$(CC) $(CFLAGS) soc_times.c -o $@

clean:
	rm -f revdns soc_times
