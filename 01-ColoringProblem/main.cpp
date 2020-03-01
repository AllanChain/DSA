#include <iostream>

short relation[13][13] = {
    /*      AB AC AD BA BC BD DA DB DC EA EB EC ED */
    /*AB*/ {0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0},
    /*AC*/ {0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0},
    /*AD*/ {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0},
    /*BA*/ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    /*BC*/ {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0},
    /*BD*/ {1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0},
    /*DA*/ {1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0},
    /*DB*/ {0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0},
    /*DC*/ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    /*EA*/ {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    /*EB*/ {0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0},
    /*EC*/ {0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0},
    /*ED*/ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
const char *names[13] = {"AB", "AC", "AD", "BA", "BC", "BD", "DA",
                         "DB", "DC", "EA", "EB", "EC", "ED"};

int main(int argc, char const *argv[]) {
  short colorIndex = 0;
  short i, j;
  bool needColor = true;
  while (needColor) {
    needColor = false;
    colorIndex++;
    for (i = 0; i < 13; i++) {
      if (relation[i][i] == 0) {
        needColor = true;
        for (j = 0; j < 13; j++)
          if (relation[j][j] == colorIndex && relation[i][j] == 1)
            break; // Conflict with existing
        if (j == 13)
          relation[i][i] = colorIndex;
      }
    }
  }
  // colorIndex will over add in the last loop
  for (i = 1; i < colorIndex; i++) {
    for (j = 0; j < 13; j++)
      if (relation[j][j] == i)
        std::cout << names[j] << ' ';
    std::cout << '\n';
  }
  return 0;
}
