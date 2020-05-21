#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int length;
  int *record;
} HeapList;

void heapSort(HeapList *list);

typedef struct RadixNode{
  int key[2];
  struct RadixNode *next;
} RadixNode;

typedef struct {
  RadixNode *f;
  RadixNode *e;
} RadixQueue;

typedef RadixNode *RadixList;

void radixSort(RadixList *list);
