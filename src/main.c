#include"nw_evtloop.h"
#include<stdlib.h>

int
main() {
	int a = 10, b = 20;
	listhead head;
	
	list_init(head);
	printf("init list head.\n");

	listnode *node = (listnode *) malloc(sizeof(listnode));
	node->ptr = &a;
	node->next = NULL;

	list_insert(head, node);

	listhead tmp = head;

	list_dump(head);

	listnode *node2 = (listnode *) malloc(sizeof(listnode));
	node2->ptr = &b;
	node2->next = NULL;

	list_insert(head, node2);
	list_dump(head);

	listnode *node3;

	list_tail(head, node3);

	printf("remove\n");	
	list_remove(&head, node);

	list_dump(head);

	list_remove(&head, node2);

	list_dump(head);

	myprinf();
	return 0;
}