#ifndef NW_EVTLOOP
#define NW_EVTLOOP
#include<stdio.h>
#include"nw_list.h"

#define EVT_READ 	0x001
#define EVT_WRITE 	0x010

#define set_read_mask(events) 	events |=0x001
#define set_write_mask(events) 	events |=0x010

typedef struct evtobj{
	int fd;
	int events;	
	void (*whandler)(struct evtobj *ev, char *buff, size_t size);
	void (*rhandler)(struct evtobj *ev, char *buff, size_t size);
}evtobj;

typedef struct _evtqueue{
	int maxsize;
	int freesize;
	listhead *evtlist;	
}evtqueue;

evtqueue *evtlp_init(int maxsize);

int evtlp_add(evtqueue *eq, evtobj *ev);

int evtlp_remove(evtqueue *eq, evtobj *ev);

void evtlp_destroy(evtqueue* eq);

void myprinf();

#endif// end of NW_EVTLOOP
