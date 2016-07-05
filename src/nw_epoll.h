#ifndef NW_EPOLL
#define NW_EPOLL

#include "nw_evtloop.h"
#include <sys/epoll.h>

extern evtqueue evt_epoll_ojb;

evtqueue *evtlp_epoll_init(evtqueue *eq, int maxsize);

int evtlp_epoll_add(evtqueue *eq, evtobj *ev);

int evtlp_epoll_remove(evtqueue *eq, evtobj *ev);

int evtlp_epoll_run(evtqueue* eq, int timeout);

void evtlp_epoll_reset(evtqueue* eq);

#endif