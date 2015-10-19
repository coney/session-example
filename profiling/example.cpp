#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <list>

__attribute__((noinline)) int ELFHash(const int value)
{
 unsigned int hash = 0;
 unsigned int x = 0;
 char *str = (char *)&value;


 while (str != (char *)(&value + 1))
 {
  hash = (hash << 4) + (*str++);
  if ((x = hash & 0xF0000000L) != 0)
  {
   hash ^= (x >> 24);
   hash &= ~x;
  }
 }

 return hash;
}


#define MAX_NUMBER  (70000)

static std::list<int> cached_values;

__attribute__((noinline)) int calculate_directly(int number) {
  if (number % 10000 == 0) {
    sleep(1);
  }
  return ELFHash(number);
}

__attribute__((noinline)) int calculate_by_cache(int number) {
  std::list<int>::const_iterator it = cached_values.begin();
  for (size_t i = 0; i < number; ++i, ++it);
  return *it;
}

__attribute__((noinline)) void init_cached_values() {
  for (size_t i = 0; i < MAX_NUMBER; i++) {
    cached_values.push_back(ELFHash(i));
  }
}

#ifdef GPERF
#include <gperftools/profiler.h>
#endif

int main(int argc, char **argv) {
#ifdef GPERF
  ProfilerStart("/tmp/e.out");
#endif
  srand(9527);
  init_cached_values();

  int result[2] = {0};
  for (size_t i = 0; i < MAX_NUMBER; i++) {
    result[0] += calculate_directly(i);
    result[1] += calculate_by_cache(i);
  }
  printf("result: %08X %08X\n", result[0], result[1]);

#ifdef GPERF
  ProfilerStop();
#endif
}
