#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>

void usage(void);
void process_file(const char *);
void connect_times(const char *);

int connect_flag=0;
int flag=0;
int port=21;

void usage(void) {
	fprintf(stdout,"\n" \
	"-c	connect only\n" \
	"-f <file>	read URL's from\n" \
	"-h	print help\n" \
	"-p	connect port\n" \
	"-?	print help\n"
	);
}

void process_file(const char * file) {
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
		connect_times(line);
	}
	

}

void connect_times(const char * host) {
	clock_t start,end;
	double elapsed;
	struct sockaddr_in servsock;
	struct in_addr serv_addr;
	int socfd;
	struct hostent * hst;
	printf("==== Now trying %s ====\n",host);
	if((socfd = socket(PF_INET,SOCK_STREAM,0)) == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}
	memset(&servsock,'\0',sizeof(servsock));
	servsock.sin_family = AF_INET;
	servsock.sin_port = htons(port);
	if(inet_aton(host,&servsock.sin_addr) == 0) {
		perror("inet_aton");
		exit(EXIT_FAILURE);
	}
	start = clock();
	if(connect(socfd,(const struct sockaddr *)&servsock,sizeof(struct sockaddr))) {
		perror("connect");
		exit(EXIT_FAILURE);
	}
	end = clock();
	close(socfd);
	elapsed = (double)( end - start);
	printf("(connect) %s elapsed:%f \n",host,elapsed);	
}

int main(int argc, char * argv[]) {
	int ch;
	char * file_name;

	while((ch = getopt(argc,argv,"cf:hp:")) != -1) {
		switch(ch) {
			case 'c':
					connect_flag = 1;
			case 'f':
					file_name = optarg;
					break;
			case 'p':
					port = atoi(optarg);
			case 'h':
			case '?':
			default:
					usage();
			
		}
	}
	
	if(file_name) {
		process_file(file_name);
	}
	return 0;		
}
