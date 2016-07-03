#ifndef NW_EVTLOOP
#define NW_EVTLOOP
#include<stdio.h>

typedef struct evtobj{
	int events;	
	void (*writehandle)(struct evtobj *ev, char *buff, size_t size);
	void (*readhandle)(struct evtobj *ev, char *buff, size_t size);
}evtobj;

typedef struct _evtmg{
	int maxevts;	
	evtobj *evtlist;	
}evtmg;

evtmg *evtlp_init();

int evtlp_add(evtmg *evtmg, evtobj *ev);

int evtlp_remove(evtmg *evtmg, evtobj *ev);

void evtlp_destroy(evtmg* evt);

void myprinf();

#endif// end of NW_EVTLOOP
