#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>
#include <ctype.h>
#include <sys/time.h>

void usage(void);
void process_file(const char *,const char *);
void connect_times(const char *,const char *);
char * trimwhitespace(char *);
void print_timediff(const struct timespec *,const struct timespec *);

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

void print_timediff(const struct timespec * end, const struct timespec * start) {
	time_t sec = (end->tv_sec) - (start->tv_sec);
	long nsec;
	if(((end->tv_nsec) - (start->tv_nsec)) < 0) {
		sec -= 1;
		nsec = 100000000 - ((end->tv_nsec) - (start->tv_nsec));
	} else {
		nsec = ((end->tv_nsec) - (start->tv_nsec));
	}
	printf("Elapsed: %lu (sec) %lu (nsec)\n",(long)sec,nsec);
}

void process_file(const char * file,const char * port) {
	FILE * fp;
	char *line,*line2,*buf;
	if((fp = fopen(file,"r")) == NULL) {
		perror("fopen");
		exit(EXIT_FAILURE);
	}
	while(!feof(fp)){
		int len;
		line = (char *)calloc(1, 128 * sizeof(char));
		buf = fgets(line,128,fp);
		if(!buf) { return; }
		len = strlen(line);
		line[len-1] = '\0';
		line2 = trimwhitespace(line);
		if(line2) connect_times(line2,port);
		free(line);
	}
	

}

void connect_times(const char * host,const char * port) {
	struct timespec start,end;
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
		if(res->ai_canonname) { 
			printf("%s\n",res->ai_canonname);
		}
		sockfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
		if(sockfd < 0) {
			perror("socket");
			continue;
		}
		err = clock_gettime(CLOCK_REALTIME,&start);
		if(err) { perror("clock_gettime"); }
		if(connect(sockfd,res->ai_addr,res->ai_addrlen)) {
			perror("connect");
			continue;
		}
		err = clock_gettime(CLOCK_REALTIME,&end);
		if(err) {
			printf("%s\n",gai_strerror(err));
			return;
		}
		close(sockfd);
		print_timediff(&end,&start);
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
					return EXIT_SUCCESS;
		}
	}
	
	if(file_name && port) {
		process_file(file_name,port);
		return EXIT_SUCCESS;;
	} 
	usage();
	return EXIT_FAILURE;		
}

char * trimwhitespace(char * str) {
	char * end;
	while(isspace(*str)) str++;

	if(*str == 0)
		return NULL;

	end = str + strlen(str) - 1;
	while(end > str && isspace(*end)) end--;
	*(end+1) = '\0';

	return str;
}
