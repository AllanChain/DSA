#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXINT 65535
#define MAXLEN 16

typedef struct LinkChar {
  char c;
  int w;
  short code[MAXLEN];
  struct LinkChar *next;
} LinkChar;

// Short for Huffman Tree Node
typedef struct {
  int w; // weight
  int parent, rlink, llink;
} HNode;

typedef struct {
  int m;       // Total outer nodes
  int root;    // indec of root
  HNode *hArr; // Array to store entire tree
} HTree;

void no_space_exit() {
  printf("Out of memory :)");
  exit(1);
}

int LinkChar_len(LinkChar *lc) {
  int m = 0;
  while ((lc = lc->next))
    m += 1;
  return m;
}

HTree *create_huffman(int m, LinkChar *lc_head) {
  HNode *node;
  LinkChar *lc = lc_head;
  int min1w, min1i, min2w, min2i;
  HTree *htree = (HTree *)malloc(sizeof(HTree));
  if (htree == NULL)
    no_space_exit();
  htree->root = 2 * m - 2; // length is 2 * m - 1, -1 again for index
  htree->hArr = (HNode *)malloc(sizeof(HNode) * (2 * m - 1));
  if (htree->hArr == NULL)
    no_space_exit();
  /** Store the tree in 2 * m - 1 array
   *
   * First m elements are used to store outer nodes a.k.a. chars
   * Last m -1 elements are inner nodes
   */
  // Set initial values
  lc = lc_head;
  for (int i = 0; i < 2 * m - 1; i++) {
    node = &(htree->hArr[i]);
    node->llink = node->rlink = node->parent = -1;
    if (i < m) {
      lc = lc->next;
      node->w = lc->w;
    } else
      node->w = -1;
  }
  // For every inner nodes, find their children
  for (int i = m; i < 2 * m - 1; i++) {
    // Finding min and second min in hArr, without parent set
    min1w = min2w = MAXINT;
    min1i = min2i = -1;
    for (int j = 0; j < i; j++) {
      node = &(htree->hArr[j]);
      if (node->parent != -1)
        continue;
      if (node->w < min1w) {
        // smaller than previous min1, so previous min1 becomes min2
        min2w = min1w;
        min2i = min1i;
        min1w = node->w;
        min1i = j;
      } else if (node->w < min2w) {
        // between min1 and min2, only min2's business
        min2w = node->w;
        min2i = j;
      }
    }
    // Set up parent and children link
    htree->hArr[min1i].parent = htree->hArr[min2i].parent = i;
    node = &(htree->hArr[i]);
    node->w = min1w + min2w;
    node->llink = min1i;
    node->rlink = min2i;
  }
  lc = lc_head;
  for (int i = 0; i < m; i++) {
    lc = lc->next;
    int j = 0, k = i;
    while (htree->hArr[k].parent != -1) {
      lc->code[j++] = htree->hArr[htree->hArr[k].parent].llink == k ? 1 : 0;
      k = htree->hArr[k].parent;
    }
  }
  return htree;
}

void add_freq(LinkChar *head, char c) {
  LinkChar *lc = head;
  while (lc->next) {
    lc = lc->next;
    if (lc->c == c) {
      lc->w += 1;
      return;
    }
  }
  lc->next = (LinkChar *)malloc(sizeof(LinkChar));
  if (lc->next == NULL)
    no_space_exit();
  lc->next->c = c;
  lc->next->w = 1;
  lc->next->next = NULL;
  for (int i = 0; i < MAXLEN; i++)
    lc->next->code[i] = -1;
}

LinkChar *char_freq(FILE *f) {
  LinkChar *head, *lc;
  head = (LinkChar *)malloc(sizeof(LinkChar));
  if (head == NULL)
    no_space_exit();
  while (!feof(f))
    add_freq(head, fgetc(f));
  return head;
}

int main(int argc, char const *argv[]) {
  FILE *f = fopen("CCode.txt", "r");
  if (f == NULL) {
    printf("NO such a file!");
    exit(2);
  }
  LinkChar *lc;
  HTree *htree;
  lc = char_freq(f);
  int m = LinkChar_len(lc);
  htree = create_huffman(m, lc);
  while (lc->next) {
    lc = lc->next;
    printf("[%d]: ", lc->c);
    int j = 0;
    while (lc->code[j++] != -1)
      ;
    for (j -= 2; j >= 0; j--)
      printf("%d", lc->code[j]);
    putchar('\n');
  }
  return 0;
}
