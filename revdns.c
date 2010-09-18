#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>

int resolve_pool(char * pool[]) {
	char * IP;
	int ip,i;
	IP = (char *)calloc(1,sizeof(char)*16);
	for(i=0;i<256;i++) {
		in_addr_t addr;
		struct hostent * hst;
		sprintf(IP,"%d.%d.%d.%d%c",atoi(pool[0]),atoi(pool[1]),atoi(pool[2]),i,'\0');
		addr = inet_addr(IP);
		hst = gethostbyaddr(&addr,sizeof(in_addr_t),AF_INET);
		if(hst)
			if(hst->h_name)
				printf("%s: %s\n",IP,hst->h_name);
		
	}
	return 0;
}

int main(int argc,char * argv[]) {
	char * pool[] = { 0, 0 , 0 };
	int i;
	if(argc<4) {
		exit(EXIT_FAILURE);
	}
	for(i=0;i<3;i++) {
		pool[i] = argv[i+1];
	}
	return resolve_pool(pool);
}
