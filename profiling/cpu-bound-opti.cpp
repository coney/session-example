#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <map>

#define MAX_NUMBER  (100000)

static std::map<int, int> cached_values;

int calculate(int number) {
  return cached_values[number];
}

void init_cached_values() {
  for (size_t i = 0; i < MAX_NUMBER; i++) {
    cached_values[i] = rand();
  }
}

int main(int argc, char **argv) {
  srand(9527);
  init_cached_values();

  unsigned long long result = 0;
  for (size_t i = 0; i < MAX_NUMBER; i++) {
    result += calculate(i);
  }

  printf("result:%llX\n", result);
}
