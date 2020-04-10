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
  char c;
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
      node->c = lchar->c;
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

LinkCharNode *LinkCharNode_find(LinkCharNode *head, char c) {
  LinkCharNode *lchar = head;
  while (lchar->next) {
    lchar = lchar->next;
    if (lchar->c == c)
      return lchar;
  }
  lchar->next = (LinkCharNode *)malloc(sizeof(LinkCharNode));
  lchar = lchar->next;
  if (lchar == NULL)
    no_space_exit();
  lchar->c = c;
  lchar->w = 0;
  lchar->next = NULL;
  lchar->code = NULL;
  return lchar;
}

LinkCharNode *char_freq(FILE *f) {
  LinkCharNode *head;
  head = (LinkCharNode *)malloc(sizeof(LinkCharNode));
  if (head == NULL)
    no_space_exit();
  while (!feof(f))
    LinkCharNode_find(head, fgetc(f))->w++;
  return head;
}

/** Write a bit into file
 *
 * Will cache bits until reached 1 byte.
 * If bit is -1, clean up byte.
 * And don't worry, will distinguish file ending with EOF code
 */
void write_bit(FILE *f, short bit) {
  static short byte = 0;
  static short bit_index = 7;

  if (bit == -1) {
    fputc(byte, f);
    return;
  }
  byte += bit << bit_index;
  bit_index--;
  if (bit_index == -1) {
    fputc(byte, f);
    byte = 0;
    bit_index = 7;
  }
}

short read_bit(FILE *f) {
  static short byte = 0;
  static short bit_index = -1;
  short result;

  if (bit_index == -1) {
    byte = fgetc(f);
    bit_index = 7;
  }
  result = (byte & (1 << bit_index)) >> bit_index;
  bit_index--;
  return result;
}

void write_huffman(LinkCharNode *lchar_head, FILE *f_orig, FILE *f_bin) {
  LinkCodeNode *lcode;
  while (!feof(f_orig)) {
    lcode = LinkCharNode_find(lchar_head, fgetc(f_orig))->code;
    while (lcode) {
      write_bit(f_bin, lcode->bit);
      lcode = lcode->next;
    }
  }
  write_bit(f_bin, -1);
}

void tran_huffman(HTree *htree, FILE *f_bin, FILE *f_tran) {
  short bit;
  int tree_index, tmp;
  bit = read_bit(f_bin);
  while (1) {
    tree_index = htree->root;
    while (1) {
      tmp = bit ? htree->hArr[tree_index].llink : htree->hArr[tree_index].rlink;
      if (tmp == -1)
        break;
      tree_index = tmp;
      bit = read_bit(f_bin);
    }
    if (htree->hArr[tree_index].c == -1)
      break;
    fputc(htree->hArr[tree_index].c, f_tran);
  }
}

int compare_file(FILE *f1, FILE *f2) {
  char c1 = 0, c2 = 0;
  while (c1 == c2 && !feof(f1) && !feof(f2)) {
    c1 = fgetc(f1);
    c2 = fgetc(f2);
  }
  return c1 - c2;
}

void print_dict(LinkCharNode *lchar) {
  LinkCodeNode *lcode;
  while (lchar->next) {
    lchar = lchar->next;
    lcode = lchar->code;
    printf("[%3d]: ", lchar->c);
    int j = 0;
    while (lcode) {
      printf("%d", lcode->bit);
      lcode = lcode->next;
    }
    putchar('\n');
  }
}

int main(int argc, char const *argv[]) {
  FILE *f_orig, *f_bin, *f_tran;
  LinkCharNode *lchar;

  HTree *htree;

  f_orig = fopen("CCode.txt", "r");
  if (f_orig == NULL) {
    printf("No such a file!");
    exit(2);
  }
  lchar = char_freq(f_orig);
  fclose(f_orig);
  int m = LinkCharNode_len(lchar);
  htree = create_huffman(m, lchar);
  generate_dict(htree, lchar);
  print_dict(lchar);

  f_orig = fopen("CCode.txt", "r");
  f_bin = fopen("CCodeBin", "w");
  write_huffman(lchar, f_orig, f_bin);
  fclose(f_orig);
  fclose(f_bin);

  f_bin = fopen("CCodeBin", "r");
  f_tran = fopen("CCodeTran", "w");
  tran_huffman(htree, f_bin, f_tran);
  fclose(f_bin);
  fclose(f_tran);

  f_orig = fopen("CCode.txt", "r");
  f_tran = fopen("CCodeTran", "r");
  printf("File diff: %d\n", compare_file(f_orig, f_tran));
  fclose(f_orig);
  fclose(f_tran);

  return 0;
}
