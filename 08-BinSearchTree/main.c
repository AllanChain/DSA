#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct BinSearchNode {
  int key;
  int data;
  struct BinSearchNode *llink, *rlink;
} BinSearchNode;

typedef struct BinSearchNode * BinSearchTree;

/** search `key` in `ptree`
 * @returns  true if found
 * @returns{pos} the found node or the last node
 * @returns{parentPos} parent of pos if found, else not modified
 */
bool search(BinSearchTree *ptree, int key,
            BinSearchNode **pos, BinSearchNode **parentPos) {
  BinSearchNode *p, *parentp;
  p = *ptree; // get BinSearchNode from BinSearchTree
  parentp = NULL;
  while (p != NULL) { // while there is still somewhere to go
    if (p->key == key) {
      *pos = p;
      if (parentPos != NULL) // if want to return parent
        *parentPos = parentp;
      return true;
    }
    parentp = p; // store `p` in `parentp` before moving forward
    if (p->key > key) {
      // key to search is smaller, go left
      p = p->llink;
    } else {
      p = p->rlink;
    }
  }
  *pos = parentp; // `p` is NULL
  return false;
}

/** insert `key` into `ptree`
 * @returns{status} 0: normal; 1: exist; 2: created
 */
int insert(BinSearchTree *ptree, int key, int data) {
  BinSearchNode *p, *pos;
  if (search(ptree, key, &pos, NULL)) return 1; // already there
  p = (BinSearchNode *)malloc(sizeof(BinSearchNode));
  if (p == NULL) {
    printf("Out of memory :)");
    exit(1);
  }
  p->key = key;
  p->data = data;
  p->llink = p->rlink = NULL;
  if (pos == NULL) {
    // Empty tree, make `p` root
    *ptree = p;
    return 2;
  }
  if (key < pos->key) pos->llink = p;
  else pos->rlink = p; // key is larger, go right
  return 0;
}

/** Delete a `key` from `ptree`
 * @returns{status} 0 for success; 1 for not found;
 */
int del(BinSearchTree *ptree, int key) {
  BinSearchNode *parentp, *p, *r;
  if (!search(ptree, key, &p, &parentp)) {
    return 1; // not found
  }
  if (p->llink == NULL) { // no left child
    /** replacing `p` with its right child,
     * different if `p` is root, right child or left child
     */
    if (parentp == NULL) { // p is root
      /** Note that if tree is empty, key will not be found
       *  Thus there is no need to handle this case
       */
      *ptree = p->rlink; // be root!
    } else if (parentp->llink == p) {
      parentp->llink = p->rlink; // connect `p`'s right child to parent
    } else { // same above
      parentp->rlink = p->rlink;
    }
  } else { // have left child, a little bit more complex
    BinSearchNode *r;
    /** get right-most node in the left child tree `r`,
     * namely the largest one smaller than `p`
     * move the right child tree (all larger ones) of `p` to `r`'s right
     * and replace `p` with `p`'s left child tree
     */
    r = p->llink; // go to left child
    while (r->rlink != NULL) r = r->rlink; // get to right most
    r->rlink = p->rlink; // move the right child tree to `r`'s right
    // replacing `p` with its left child
    if (parentp == NULL) {
      *ptree = p->llink;
    } else if (parentp->llink == p) {
      parentp->llink = p->llink;
    } else {
      parentp->rlink = p->llink;
    }
  }
  free(p);
  return 0;
}

/** Core function for REPL
 */
void evaluate(BinSearchTree *ptree) {
  char line[64], command[16];
  int key, data;
  BinSearchNode *pos;

  fgets(line, 64, stdin);
  sscanf(line, "%s %d", command, &key);
  if (strcmp(command, "insert") == 0) {
    sscanf(line, "%s %d:%d", command, &key, &data);
    switch (insert(ptree, key, data)) {
      case 0: 
        printf("Inserted %d with data %d\n", key, data);
        break;
      case 1:
        printf("Did nothing, key already exist.\n");
        break;
      case 2:
        printf("Created tree, root is %d with data %d\n", key, data);
    }
  } else if (strcmp(command, "search") == 0){
    if (search(ptree, key, &pos, NULL))
      printf("Found %d with data %d\n", key, pos->data);
    else
      printf("Not found\n");
  } else if (strcmp(command, "del") == 0){
    switch (del(ptree, key)) {
      case 0:
        printf("Deleted %d with data %d\n", key, pos->data);
        break;
      case 1:
        printf("Not found\n");
    }
  } else if (strcmp(command, "exit") == 0) {
    printf("logout\n");
    exit(0);
  } else {
    printf("Commands:\n");
    printf("\tinsert key:value\n");
    printf("\tsearch key\n");
    printf("\tdel key\n");
    printf("\texit\n");
  }
}

int main(int argc, char const *argv[]) {
  BinSearchTree ptree = NULL;
  while (true) evaluate(&ptree);
  return 0;
}
