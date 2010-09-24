#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <stdlib.h>

#include "hostlist.h"

static hostnode * smallest;
static int least_inited = 0;
extern int verbose;

int addhost(hostnode ** head,hostnode * node) {
	if(!head) {
		return 0;
	}
	if(node) {
		node->next = *head;
		*head = node;
		return 1;
	}
	return 0;
}

hostnode * newhostnode(const char * host, const struct timespec * tm) {
	hostnode * node = NULL;
	node = (hostnode *)malloc(sizeof(hostnode));
	if(!node)
		return NULL;
	node->host = strdup(host);
	node->tm = *tm;
	node->next = NULL;
	return node;
}

void free_hostlist(hostnode * head) {
	while(head) {
		hostnode * node = head->next;
		free(head->host);
		free(head);
		head = node;
	}
}

void print_hostlist(hostnode * head) {
	hostnode * list = head;
	for(list = head; list ; list = list->next) {
		fprintf(stderr,"%s\n",list->host);
	}
}

static void compare_and_swap_smallest(hostnode * node) {
	struct timespec sml,cur;
	sml = smallest->tm;
	cur = node->tm;

	if(cur.tv_sec < sml.tv_sec) {
		smallest = node;
	} else if ( cur.tv_sec == sml.tv_sec ) {
		if(cur.tv_nsec < sml.tv_nsec)
			smallest = node;
	}
}

void find_smallestnode(hostnode * head) {
	hostnode * list = head;
	for(list = head; list ; list = list->next) {
		if(!least_inited) {
			// Insert the first node;
			smallest = list;
			least_inited = 1;
		}
		//compare this node with smallest;
		compare_and_swap_smallest(list);
	}
	if(verbose) printf("Smallest connect(2) time is for %s at ( %lu secs and %lu nano secs )\n",smallest->host,(unsigned long)smallest->tm.tv_sec,(unsigned long)smallest->tm.tv_nsec); 	
	printf("%s\n",smallest->host);
}


