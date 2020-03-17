#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// typedef struct {
//   int location;
//   Node *next;
// } Node;

// typedef struct {
//   Node *top;
// } LinkStack;

// LinkStack *stackCreate() {
//   LinkStack *stack;
//   stack = (LinkStack *)malloc(sizeof(LinkStack));
//   stack->top = NULL;
//   return stack;
// }

// void stackPush(LinkStack *stack, int location) {
//   Node *node = (Node *)malloc(sizeof(Node));
//   node->location = location;
//   node->next = stack->top;
//   stack->top = node;
// }

// int stackPop(LinkStack *stack) {
//   Node *node = stack->top;
//   stack->top = node->next;
//   return node->location;
// }

// int stackPeek(LinkStack *stack) { return stack->top->location; }

// bool stackEmpty(LinkStack *stack) { return stack->top == NULL; }

bool get(int location, int item) { return (location & item) != 0; }

const int farmer = 0x08;
const int wolf = 0x04;
const int goat = 0x02;
const int cabbage = 0x01;

bool safe(int location) {
  if (get(location, goat) != get(location, farmer) &&
      (get(location, goat) == get(location, cabbage) ||
       get(location, goat) == get(location, wolf)))
    return false;
  return true;
}

int main(int argc, char const *argv[]) {
  int movers, location, newLocation;
  int route[16]; // record location before
  bool done = false;
  // LinkStack *moveStack = stackCreate();
  // stackPush(moveStack, 0x00);
  location = 0x00;
  route[0] = 0;
  for (int i = 1; i < 16; i++) {
    if (safe(i))
      route[i] = -1;
    else
      route[i] = -2;
  }
  while (!done) {
    // location = stackPeek(moveStack);
    // try to move all items
    for (movers = 1; movers <= 8; movers <<= 1) {
      // if farmer and item are at the same place
      // that is, farmer can bring it
      if (get(location, farmer) == get(location, movers)) {
        // `farmer | movers` means farmer and the item to move
        // XOR means to flip their location
        newLocation = location ^ (farmer | movers);
        if (newLocation == 0x0f) {
          route[newLocation] = location;
          done = true;
          break;
        }
        // if safe and unvisited
        if (route[newLocation] == -1) {
          route[newLocation] = location;
          location = newLocation;
          break;
        }
      }
    }
    if (movers == 16) { // no break, no further solution
      // no where to step back, no solution
      if (location == 0) {
        // don't worry, we will distinguish solution vs nosolution
        // via the value of route[0x0f]
        done = true;
      }
      // no need to clear route, just mark as visited
      location = route[location];
    }
  }
  if (route[0x0f] == -1) {
    printf("no solution");
  } else {
    location = 0x0f;
    while (location) {
      printf("%x ", location);
      location = route[location];
    }
    printf("0\n");
  }
  return 0;
}
