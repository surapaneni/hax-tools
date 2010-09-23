#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>

void usage(void);
void process_file(const char *,const char *);
void connect_times(const char *,const char *);

int connect_flag=0;
int flag=0;

void usage(void) {
	fprintf(stdout,"\n" \
	"-c	connect only\n" \
	"-f <file>	read URL's from\n" \
	"-h	print help\n" \
	"-p	connect port\n" \
	"-?	print help\n"
	);
}

void process_file(const char * file,const char * port) {
	FILE * fp;
	char line[128];
	if((fp = fopen(file,"r")) == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}
	while(!feof(fp)){
		int len;
		fgets(line,sizeof(line),fp);
		len = strlen(line);
		line[len-1] = '\0';
		connect_times(line,port);
	}
	

}

void connect_times(const char * host,const char * port) {
	clock_t start,end;
	struct addrinfo *res,*res0,hints;
	int err;
	printf("Now connecting to %s on port %s\n",host,port);
	memset(&hints,0,sizeof(struct addrinfo));
	hints.ai_family = PF_INET;
	hints.ai_socktype = SOCK_STREAM;
	err = getaddrinfo(host,port,&hints,&res0);
	if(err) {
		printf("%s\n",gai_strerror(err));
		return;
	}
	for(res=res0;res;res=res->ai_next) {
		int sockfd;
		sockfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
		if(sockfd < 0) {
			perror("socket");
			continue;
		}
		start = clock();
		if(connect(sockfd,res->ai_addr,res->ai_addrlen)) {
			perror("connect");
			continue;
		}
		end = clock();
		close(sockfd);
		printf("Elapsed: %lf\n",(double)(end - start));
	}
	freeaddrinfo(res0);
}

int main(int argc, char * argv[]) {
	int ch;
	char * file_name = NULL;
	char * port = NULL;
	while((ch = getopt(argc,argv,"chf:p:")) != -1) {
		switch(ch) {
			case 'c':
					connect_flag = 1;
					break;
			case 'f':
					file_name = optarg;
					break;
			case 'p':
					port = optarg;
					break;
			case 'h':
			case '?':
			default:
					usage();
			
		}
	}
	
	if(file_name) {
		process_file(file_name,port);
	}
	return 0;		
}
