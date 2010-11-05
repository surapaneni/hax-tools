#ifndef _HOSTLIST_H_
#define _HOSTLIST_H_

typedef struct hostnode hostnode;
struct hostnode {
	char * host;
	struct timespec tm;
	struct hostnode * next;
};

int addhost(hostnode **,hostnode *);
hostnode * newhostnode(const char *, const struct timespec *);
void print_hostlist(hostnode *);
void find_smallestnode(hostnode *);
void free_hostlist(hostnode *);
#endif


