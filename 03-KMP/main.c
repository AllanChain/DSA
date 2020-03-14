#include <stdio.h>

typedef struct {
  int n;   // string length
  char *c; // string chars
} String;

int find(String *target, String *pattern, int *next) {
  int i = 0, j = 0;
  while (i < pattern->n && j < target->n) {
    if (i == -1 || pattern->c[i] == target->c[j]) {
      i++;
      j++;
    } else {
      i = next[i];
    }
  }
  if (i >= pattern->n)
    return (j - pattern->n + 1);
  else
    return -1;
}

void calcNext(String *pattern, int *next) {
  int i = 0, k = -1;
  next[0] = -1;
  while (i < pattern->n - 1) {
    while (k >= 0 && pattern->c[i] != pattern->c[k])
      k = next[k];
    i++;
    k++;
    // Meaning of i / k changed
    if (pattern->c[i] == pattern->c[k])
      next[i] = next[k];
      // as if do another k = next[k]
      // but do not `k = next[k]` because k still means
      // the longest match to the next loop
    else
      next[i] = k;
  }
}

int main(int argc, char const *argv[])
{
  char t[19] = "aasdsasdaasdaasasd";
  char p[10] = "asdaasasd";
  int next[9];
  int result;
  String target = {18, t};
  String pattern = {9, p};

  calcNext(&pattern, next);
  result = find(&target, &pattern, next);
  printf("Found at %d", result);
  return 0;
}
