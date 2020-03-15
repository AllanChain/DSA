#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const short maxKeywords = 16;
const short ignoreWordsCount = 6;
const char *ignoreWords[] = {"and", "of", "the", "to", "", "a"};

struct BookList {
  short No; // numero of the book
  struct BookList *next;
};

struct KeywordItem {
  char word[64];
  struct BookList *books;
};

struct IndexList {
  struct KeywordItem keywords[maxKeywords];
  short length;
};

short toIgnore(char *keyword) {
  for (int i = 0; i < ignoreWordsCount; i++)
    if (strcmp(ignoreWords[i], keyword) == 0)
      return 1;
  return 0;
}

char *strlwr(char *str) {
  unsigned char *p = (unsigned char *)str;
  while (*p) {
    *p = tolower((unsigned char)*p);
    p++;
  }
  return str;
}

short binarySearch(struct IndexList *indices, char *keyword) {
  // explicit is better than implicit, this is a special case
  if (indices->length == 0)
    return -0 - 1; // needs create

  short bottom = 0, top = indices->length - 1;
  short mid;
  short cmpResult;

  while (bottom <= top) {
    mid = (bottom + top) / 2;
    cmpResult = strcmp(keyword, indices->keywords[mid].word);
    if (cmpResult == 0) {
      return mid;
    } else if (cmpResult < 0) {
      // `keyword` is close to beginning
      top = mid - 1;
    } else {
      bottom = mid + 1;
    }
  }
  // if not found, 2 cases: slightly less or greater than mid
  // if just use negtive value for not found, what 0 means, found or not found?
  if (cmpResult > 0)
    return -(mid + 1) - 1;
  return -mid - 1;
}

void addBook(struct IndexList *indices, char *keyword, short no) {
  short position = binarySearch(indices, keyword);
  struct BookList *book = (struct BookList *)malloc(sizeof(struct BookList));
  book->No = no;
  book->next = NULL;
  if (position >= 0) {
    // don't do anything if already added
    if (indices->keywords[position].books->No != no) {
      // insert book
      book->next = indices->keywords[position].books;
      indices->keywords[position].books = book;
    }
  } else {
    position = -(position + 1);
    // insert item
    struct KeywordItem *item =
        (struct KeywordItem *)malloc(sizeof(struct KeywordItem));
    // If not copy, every thing will be lost after next input!
    strcpy(item->word, keyword);
    item->books = book;
    for (int i = indices->length; i > position; i--)
      indices->keywords[i] = indices->keywords[i - 1];
    indices->keywords[position] = *item;
    indices->length++;
  }
}

int main(int argc, char const *argv[]) {
  short bookNo;
  char line[256];
  char *keyword;
  struct IndexList indices;
  struct BookList *book;

  indices.length = 0;
  while (1) {
    printf("Book No: ");
    fgets(line, 256, stdin);
    sscanf(line, "%hd", &bookNo);
    if (bookNo < 0) {
      for (int i = 0; i < indices.length; i++) {
        keyword = indices.keywords[i].word;
        printf("%s: ", keyword);
        book = indices.keywords[i].books;
        while (book) {
          printf(" %hd", book->No);
          book = book->next;
        }
        printf("\n");
      }
      return 0;
    }
    printf("Book Name: ");
    fgets(line, 256, stdin);
    printf("\033[0;32mAdded %s\033[0m", line);
    keyword = strtok(line, " \n");
    while (keyword != NULL) {
      keyword = strlwr(keyword);
      if (!toIgnore(keyword)) {
        addBook(&indices, keyword, bookNo);
      }
      keyword = strtok(NULL, " \n");
    }
  }
  return 0;
}
