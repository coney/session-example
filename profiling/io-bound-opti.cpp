#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread>
#include <queue>


#define MAX_NUMBER    (100000)

#define THREAD_COUNT  64

unsigned long long result = 0;
::std::queue<int> per_thread_results[THREAD_COUNT];

int calculate(int i) {
  usleep(1);
  return rand();
}

void *calc_proc(void *arg) {
  int number =(int)(long)arg;

}

int main(int argc, char **argv) {
  srand(9527);
  for (size_t i = 0; i < THREAD_COUNT; i++) {
  }

  printf("result:%llX\n", result);
}
