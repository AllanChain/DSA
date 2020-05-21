#include "lib.h"

/**
 * Building the heap from p to size.
 * As sorting, the last few elements does not belong to the heap,
 *     need to be excluded via `size`
 */
void sift(HeapList *list, int size, int p) {
  int temp = list->record[p];
  int child = 2 * p + 1; // left child
  while (child < size) { // in the heap
    if (child < size - 1 && list->record[child] < list->record[child + 1]) {
      // only use right when: 1. inside heap; 2. larger
      child++;
    }
    if (temp < list->record[child]) {
      list->record[p] = list->record[child];
      /* You may want to put temp into list->record[child]
       * But you need get list->record[child] as temp right after
       * Just do nothing here.
       * Or in another perspective, you are moving the same number
       *     around the heap, no need to put it down before all settled
       */
      p = child;
      child = 2 * p + 1;
    } else
      break;                // All settled
    list->record[p] = temp; // Note `p` was `child` in prev loop
  }
}

void heapSort(HeapList *list) {
  int temp;
  // Build initial heep from last non-leaf node
  for (int i = list->length / 2 - 1; i >= 0; i--)
    sift(list, list->length, i);
  for (int i = list->length - 1; i > 0; i--) {
    // Swap heap top with the last record
    // namely move the large one to back
    temp = list->record[0];
    list->record[0] = list->record[i];
    list->record[i] = temp;
    sift(list, i, 0);
  }
}
