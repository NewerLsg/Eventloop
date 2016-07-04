#include "nw_list.h"

void 
list_remove(listhead *head, listnode *node) {
	if (*head == NULL || node == NULL) return ;

	if (node == *head) {
		*head = NULL;
		return ;
	}

	listnode *tmp = *head;

	while(tmp && tmp->next != node) list_next(node);

	if (tmp != NULL) tmp ->next = node->next;
}

void 
list_dump(listhead head) 
{
	listhead tmp = head;

	while (tmp) {
		printf("cur:[%p], next:[%p]\n", tmp, tmp->next);
		list_next(tmp);
	}
}