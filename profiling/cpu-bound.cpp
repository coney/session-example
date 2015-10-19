#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <list>

#define MAX_NUMBER  (100000)

static std::list<int> cached_values;

__attribute__((noinline)) int calculate(int number) {
  std::list<int>::const_iterator it = cached_values.begin();
  for (size_t i = 0; i < number; ++i, ++it);
  return *it;
}

void init_cached_values() {
  for (size_t i = 0; i < MAX_NUMBER; i++) {
    cached_values.push_back(rand());
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
