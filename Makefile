CC=	gcc
CFLAGS=	-std=c99

tools:	revdns

revdns:	revdns.c
	$(CC) $(CFLAGS) revdns.c -o $@

clean:
	rm -f revdns
