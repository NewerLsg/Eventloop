#ifndef NW_EVTLOOP
#define NW_EVTLOOP
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "nw_list.h"

#define EVT_READ 	0x001
#define EVT_WRITE 	0x0

#define set_read_mask(events) 	events |=0x001
#define set_write_mask(events) 	events |=0x010

#define eventloop_init(maxsize)  evtlp->evtlp_init(evtlp,maxsize)
#define eventloop_add(ev)  evtlp->evtlp_add(evtlp, ev)
#define eventloop_remove(ev)  evtlp->evtlp_remove(evtlp, ev)
#define eventloop_run(timeout) evtlp->evtlp_run(evtlp, timeout)
#define eventloop_destroy() evtlp->evtlp_destroy(evtlp)

typedef struct evtobj{
	int fd;
	int events;	
	void (*whandler)(int fd, void *data);
	void (*rhandler)(int fd, void *data);
}evtobj;

typedef struct _evtqueue {
	int fd;
	int maxsize;
	int freesize;
	listhead evtlist;	
	struct _evtqueue * (*evtlp_init)(struct _evtqueue *,int);
	int (*evtlp_add)(struct _evtqueue *, evtobj *);
	int (*evtlp_remove)(struct _evtqueue *, evtobj *);
	int (*evtlp_run)(struct _evtqueue*, int );
	void (*evtlp_destroy)(struct _evtqueue* );
}evtqueue;

#endif// end of NW_EVTLOOP