#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>

int resolve_pool(char * pool[]) {
	char * IP;
	IP = (char *)calloc(1,sizeof(char)*16);
	for(int i=0;i<256;i++) {
		in_addr_t addr;
		struct hostent * hst;
		sprintf(IP,"%d.%d.%d.%d%c",atoi(pool[0]),atoi(pool[1]),atoi(pool[2]),i,'\0');
		addr = inet_addr(IP);
		hst = gethostbyaddr(&addr,sizeof(in_addr_t),AF_INET);
		if(hst) {
			if(hst->h_name) {
				struct hostent * fwhst;
				printf("\n%s: %s ",IP,hst->h_name);
				fwhst = gethostbyname(hst->h_name);
				if(fwhst) {
					if(fwhst->h_name) {
						printf(" FW: %s ", fwhst->h_name);
					}
					if(fwhst->h_aliases) {
						char * alias;
						printf(" ALIAS:");
						while((alias = *(fwhst->h_aliases++))) {
							printf(" %s",alias);
						}
						printf("\n");
					}
				}
				
			}
		}
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
