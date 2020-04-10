#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXINT 65535

/**
 * The last node is not an empty tail
 * to avoid special cases
 */
typedef struct LinkCodeNode {
  short bit;
  struct LinkCodeNode *next;
} LinkCodeNode;

/**
 * The head of LinkCharNode is an empty node
 * to avoid special cases
 */
typedef struct LinkCharNode {
  char c;
  int w;
  LinkCodeNode *code;
  struct LinkCharNode *next;
} LinkCharNode;

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

int LinkCharNode_len(LinkCharNode *lchar) {
  int m = 0;
  while ((lchar = lchar->next))
    m += 1;
  return m;
}

void generate_dict(HTree *htree, LinkCharNode *lchar_head) {
  LinkCharNode *lchar;
  LinkCodeNode *lcode;
  lchar = lchar_head;
  int i = 0, k;

  while (lchar->next) {
    lchar = lchar->next;
    k = i++;
    while (htree->hArr[k].parent != -1) {
      lcode = (LinkCodeNode *)malloc(sizeof(LinkCodeNode));
      lcode->bit = htree->hArr[htree->hArr[k].parent].llink == k ? 1 : 0;
      lcode->next = lchar->code;
      lchar->code = lcode;
      k = htree->hArr[k].parent;
    }
  }
}

HTree *create_huffman(int m, LinkCharNode *lchar_head) {
  HNode *node;
  LinkCharNode *lchar = lchar_head;
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
  lchar = lchar_head;
  for (int i = 0; i < 2 * m - 1; i++) {
    node = &(htree->hArr[i]);
    node->llink = node->rlink = node->parent = -1;
    if (i < m) {
      lchar = lchar->next;
      node->w = lchar->w;
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
  return htree;
}

void add_freq(LinkCharNode *head, char c) {
  LinkCharNode *lchar = head;
  while (lchar->next) {
    lchar = lchar->next;
    if (lchar->c == c) {
      lchar->w += 1;
      return;
    }
  }
  lchar->next = (LinkCharNode *)malloc(sizeof(LinkCharNode));
  if (lchar->next == NULL)
    no_space_exit();
  lchar->next->c = c;
  lchar->next->w = 1;
  lchar->next->next = NULL;
  lchar->next->code = NULL;
}

LinkCharNode *char_freq(FILE *f) {
  LinkCharNode *head, *lchar;
  head = (LinkCharNode *)malloc(sizeof(LinkCharNode));
  if (head == NULL)
    no_space_exit();
  while (!feof(f))
    add_freq(head, fgetc(f));
  return head;
}

int main(int argc, char const *argv[]) {
  FILE *f;
  LinkCharNode *lchar;
  LinkCodeNode *lcode;
  HTree *htree;

  f = fopen("CCode.txt", "r");
  if (f == NULL) {
    printf("NO such a file!");
    exit(2);
  }
  lchar = char_freq(f);
  fclose(f);
  int m = LinkCharNode_len(lchar);
  htree = create_huffman(m, lchar);
  generate_dict(htree, lchar);
  while (lchar->next) {
    lchar = lchar->next;
    lcode = lchar->code;
    printf("[%d]: ", lchar->c);
    int j = 0;
    while (lcode) {
      printf("%d", lcode->bit);
      lcode = lcode->next;
    }
    putchar('\n');
  }
  return 0;
}
