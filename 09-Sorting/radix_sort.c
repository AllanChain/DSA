#include "lib.h"

void radixSort(RadixList *list) {
  int i, j, k; // need iter variable outside loop, so define here
  // `list` has an empty head node
  RadixNode *p, *head = (*list)->next;
  RadixQueue queue[10];
  for (j = 1; j >= 0; j--) {
    // Clear the queue
    for (i = 0; i < 10; i++)
      queue[i].f = queue[i].e = NULL;
    for (p = head; p != NULL; p = p->next) {
      // Move every p in list to correct queue
      k = p->key[j];
      // Be the head if empty queue
      if (queue[k].f == NULL)
        queue[k].f = p;
      else
        (queue[k].e)->next = p;
      queue[k].e = p; // Set up end
    }
    // Find the first non-empty queue
    for (i = 0; queue[i].f == NULL; i++)
      ;
    p = queue[i].e;    // `p` points to last
    head = queue[i].f; // `head` is collected head
    for (i++; i < 10; i++) { // collect queues behind
      if (queue[i].f != NULL) { // ignore empty queue
        p->next = queue[i].f;
        p = queue[i].e;
      }
    }
    p->next = NULL; // terminate
  }
  (*list)->next = head; // "return" head
}
