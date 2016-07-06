#include "nw_epoll.h"

evtqueue evt_epoll_ojb = {
	-1, 0, 0,
	NULL,
	evtlp_epoll_init,
	evtlp_epoll_add,
	evtlp_epoll_remove,
	evtlp_epoll_run,
	evtlp_epoll_reset
};

evtqueue 
*evtlp_epoll_init(evtqueue *eq, int maxsize) 
{
	int fd;
	if ((fd = epoll_create(maxsize)) <= 0) {
		printf("init fail. %s\n", strerror(errno));
		return NULL;
	}
	eq->fd = fd; 
	eq->freesize = eq->maxsize = maxsize;
	eq->evtlist	= NULL;
	printf("loop init done.fd[%d], maxsize[%d]\n", eq->fd, eq->freesize);
	return eq;
}

int 
evtlp_epoll_add(evtqueue *eq, evtobj *ev) 
{
	if (eq == NULL || ev == NULL) return 0;

	if (eq->freesize <= 0) return -1;

	struct epoll_event evt;
	evt.events 	|= ev->events & EVT_READ == EVT_READ ? EPOLLIN : evt.events;
	evt.events 	|= ev->events & EVT_WRITE == EVT_WRITE ? EPOLLOUT : evt.events;
	evt.events  |= EPOLLET;
	evt.data.ptr = ev;

	if (-1 == epoll_ctl(eq->fd, EPOLL_CTL_ADD, ev->fd, &evt)) {
		printf("add fail.err[%s]\n", strerror(errno));
		return -1;
	}

	listnode *node = (listnode *) malloc(sizeof(listnode));

	node->next = NULL;
	node->ptr 	= ev;

	eq->freesize--;
	list_insert(eq->evtlist, node);

	printf("[%s]add event.\n", __FUNCTION__);
	return 0;
}

int 
evtlp_epoll_remove(evtqueue *eq, evtobj *ev) 
{
	if (eq == NULL || ev == NULL) return 0;

	if (eq->freesize == eq->maxsize) return -1;

	if (-1 == epoll_ctl(eq->fd, EPOLL_CTL_DEL, ev->fd, NULL) ) {
		return -1;
	}

	listnode* tmp =  list_find(eq->evtlist, ev);
	list_remove(&(eq->evtlist), tmp);
	eq->freesize++;
	return 0;
}

int  
evtlp_epoll_run(evtqueue* eq, int timeout)
{
	if (eq == NULL) return -1;

	while(1) { 
		struct epoll_event *events = (struct epoll_event *) malloc(sizeof(struct epoll_event) * eq->maxsize / 2);
		int nfds = epoll_wait(eq->fd, events, eq->maxsize / 2, timeout * 1000);

		if (nfds < 0) {
			continue;
		}

		printf("[%s]%d events happen.\n",  __FUNCTION__, nfds);

		int i = 0;
		for(; i< nfds; i++) {
			evtobj *ev = (evtobj *)(events[i].data.ptr);
			if ((events[i].events & EPOLLIN) && ev->rhandler != NULL) {
				ev->rhandler(ev->fd, NULL);
			}

			if ((events[i].events & EPOLLOUT) && ev->whandler != NULL) {
				ev->whandler(ev->fd, eq);
			}
 		}

		free(events);
	}

	return 0;
}

void 
evtlp_epoll_reset(evtqueue* eq) {
	if (eq == NULL) return;
	close(eq->fd);
	eq->fd = -1;
	eq->maxsize = eq->freesize = 0;
	list_destroy(&(eq->evtlist));
	return ;
}
