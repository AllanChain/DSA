#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define ROWS 18
#define COLS 36
#define WALL -1
#define ROAD -2
#define IN -3
#define OUT -4

typedef struct Node {
  int data;
  struct Node *link;
} Node;

typedef struct {
  Node *front;
  Node *rear;
} LinkQueue;

LinkQueue *LinkQueue_create() {
  LinkQueue *p;
  p = (LinkQueue *)malloc(sizeof(LinkQueue));
  if (p == NULL) {
    printf("Out of space! Exiting...");
    exit(-1);
  }
  p->front = NULL;
  p->rear = NULL;
  return p;
}

bool LinkQueue_empty(LinkQueue *queue) { return (queue->front == NULL); }

void LinkQueue_put(LinkQueue *queue, int item) {
  Node *nodeP;
  nodeP = (Node *)malloc(sizeof(Node));
  if (nodeP == NULL) {
    printf("Out of space! Exiting...");
    exit(-1);
  }
  nodeP->data = item;
  nodeP->link = NULL;
  if (LinkQueue_empty(queue)) {
    queue->front = nodeP;
    // Need to initiate rear
    queue->rear = nodeP;
  } else {
    queue->rear->link = nodeP;
    queue->rear = nodeP;
  }
}

int LinkQueue_get(LinkQueue *queue) {
  Node *nodeP;
  int data;
  nodeP = queue->front;
  queue->front = nodeP->link;
  data = nodeP->data;
  free(nodeP);
  return data;
}

void destruct_pos(int pos, int *row, int *col) {
  *row = pos / COLS;
  *col = pos % COLS;
}

int construct_pos(int row, int col) {
  // merge `row` and `col` to a int to be store in maze
  return (row * COLS + col);
}

int *read_maze(FILE *fp, int *in, int *out) {
  int *maze, *tmp;
  tmp = maze = (int *)malloc(sizeof(int) * ROWS * COLS);
  while (!feof(fp)) {
    switch (fgetc(fp)) {
    case '#':
      *tmp++ = WALL;
      break;
    case ' ':
      *tmp++ = ROAD;
      break;
    case 'I':
      *in = tmp - maze;
      *tmp++ = IN;
      break;
    case 'O':
      *out = tmp - maze;
      *tmp++ = OUT;
      break;
    case 'B':
      *tmp++ = OUT;
      // Do nothing if newline
    }
  }
  return maze;
}

void print_graph(int *maze, int out) {
  char map[ROWS * COLS];
  int posNew, pos, in;
  for (pos = 0; pos < ROWS * COLS; pos++) {
    switch (*(maze + pos)) {
    case WALL:
      *(map + pos) = '#';
      break;
    case IN:
      *(map + pos) = 'I';
      in = pos;
      break;
    default:
      // ROAD and walked
      *(map + pos) = ' ';
      break;
    }
  }
  *(map + out) = 'O';

  pos = out;
  posNew = *(maze + pos);
  while (posNew != in) {
    if (posNew - pos == 1 || posNew - pos == -1) {
      *(map + posNew) = '-';
    } else {
      *(map + posNew) = '|';
    }
    pos = posNew;
    posNew = *(maze + pos);
  }
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      putchar(*(map + construct_pos(i, j)));
    }
    putchar('\n');
  }
}

int main(int argc, char const *argv[]) {
  FILE *fp;
  int *maze;
  int out, pos, row, col, posNew, rowNew, colNew;
  LinkQueue *queue;
  bool done = false;

  queue = LinkQueue_create();
  fp = fopen("maze.txt", "r");
  maze = read_maze(fp, &pos, &out);
  LinkQueue_put(queue, pos);
  while (*(maze + out) == OUT && !LinkQueue_empty(queue)) {
    pos = LinkQueue_get(queue);
    destruct_pos(pos, &row, &col);
    // Loop through [(-1, 0), (0, -1), (1, 0), (0, 1)]
    for (int i = -1; i <= 1; i++) {
      for (int j = -1; j <= 1; j++) {
        if (i + j == 1 || i + j == -1) {
          rowNew = row + i;
          colNew = col + j;
          posNew = construct_pos(rowNew, colNew);
          // Out of boundary
          if (rowNew < 0 || rowNew >= ROWS || colNew < 0 || colNew >= COLS)
            continue;
          if (*(maze + posNew) == ROAD) {
            LinkQueue_put(queue, posNew);
            *(maze + posNew) = pos;
          } else if (*(maze + posNew) == OUT) {
            *(maze + posNew) = pos;
          }
        }
      }
    }
  }
  if (*(maze + out) != OUT) {
    print_graph(maze, out);
  } else {
    printf("No solution");
  }
  return 0;
}
