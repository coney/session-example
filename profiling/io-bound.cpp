#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_NUMBER  (100000)

int calculate(int i) {
  usleep(1);
  return rand();
}

int main(int argc, char **argv) {
  srand(9527);
  unsigned long long result = 0;
  for (size_t i = 0; i < MAX_NUMBER; i++) {
    result += calculate(i);
  }

  printf("result:%llX\n", result);
}
