#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include "nw_eventloop.h"

int setnonblock(int fd) {
   int fdflags;

   if ((fdflags = fcntl(fd, F_GETFL, 0)) == -1)
      return -1;
   fdflags |= O_NONBLOCK;
   if (fcntl(fd, F_SETFL, fdflags) == -1)
      return -1;
   return 0;
}

void myhandler(int fd, void *data) {
	printf("get data\n");
	return ;
	/*
	char buf[128];
	int size;

	if ((size = read(fd, buf, sizeof(buf))) < 0) {
		printf("fail to read.error[%s]\n", strerror(errno));
		return ;
	}

	printf("data[%s]\n", buf);
	return;
	*/
}

void rhandler(int fd, void *data) {
	char buf[2014];
	int csock = accept(fd, NULL, NULL);

	if (csock < 0) {
		printf("fail to accept client.err[%s]\n", strerror(errno));
		return ;
	}
	
	printf("%s:fd[%d]", __FUNCTION__, csock);
	setnonblock(csock);
	evtqueue *eq = (evtqueue *)data;

	evtobj *ev = (evtobj *)malloc(sizeof(evtobj));
	ev->fd = csock;
	ev->rhandler = myhandler;

	ev->rhandler(csock,NULL);

	printf("[%s]ev[%p],rhandler:[%p]\n",__FUNCTION__, ev, ev->rhandler);

	set_read_mask(ev->events);

	eventloop_add(ev);
	printf("add new client.");
	return;
}

int init_server() {
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	if(sock <= 0) {
		printf("failt to init server sock,err[%s]\n", strerror(errno));
		return -1;
	}

	 struct sockaddr_in add;
	 add.sin_family = AF_INET;
	 add.sin_port = htonl(10001);
	 add.sin_addr.s_addr = INADDR_ANY;
	 bzero(&(add.sin_zero), 8);

	 if (bind(sock, (struct sockaddr *)&add, sizeof(struct sockaddr)) != 0) {
	 	printf("bind fail.err[%s]\n", strerror(errno));
	 	return -1;
	 }

	if (listen(sock, 5) != 0) {
		printf("listen fail.err[%s]\n", strerror(errno));
		return -1;
	}

	setnonblock(sock);

	return sock;
}


int
main() {
	evtqueue *eq = eventloop_init(10);

	if (eq == NULL) {
		printf("init fail.\n");
		return;
	}

	int sock = init_server();

	if (sock <= 0) return 0;

	evtobj *ev = (evtobj *)malloc(sizeof(evtobj));
	ev->fd = sock;
	ev->rhandler = rhandler;

	printf("list socke handle[%p]\n",ev->rhandler);

	set_read_mask(ev->events);

	eventloop_add(ev);

	eventloop_run(10);

	printf("test done\n");

	return 0;
}
