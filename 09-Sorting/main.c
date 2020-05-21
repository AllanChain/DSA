#include "lib.h"
#define DATA                                                                   \
  { 79, 8, 6, 93, 59, 84, 55, 9, 71, 33, 16 }
#define DATA_LEN 11

void doHeapSort() {
  int records[DATA_LEN] = DATA;
  HeapList list = {DATA_LEN, records};
  heapSort(&list);
  for (int i = 0; i < DATA_LEN; i++)
    printf("%d ", records[i]);
}

void doRadixSort() {
  int records[DATA_LEN] = DATA;
  RadixNode *node = NULL, *end = NULL;
  RadixList list = (RadixNode *)malloc(sizeof(RadixNode));
  end = list;
  for (int i = 0; i < DATA_LEN; i++) {
    node = (RadixNode *)malloc(sizeof(RadixNode));
    node->key[0] = records[i] / 10;
    node->key[1] = records[i] % 10;
    end->next = node;
    end = node;
  }
  radixSort(&list);
  node = list;
  while (node->next) {
    node = node->next;
    printf("%d ", node->key[0] * 10 + node->key[1]);
  }
}

int main(int argc, char const *argv[]) {
  doHeapSort();
  putchar('\n');
  doRadixSort();
  return 0;
}
