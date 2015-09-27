#include <stdio.h>

#define ARRAY_SIZE	10

    static int arr[ARRAY_SIZE] = { 0 };

    long addAll(long v1, long v2, long v3, long v4,
        long v5, long v6, long v7, long v8)
    {
        return v1 + v2 + v3 + v4 + v5 + v6 + v7 + v8;
    }

    int main(int argc, char **argv) {

      for (int i = 0; i < ARRAY_SIZE; ++i) {
        arr[i] = i;
      }

      printf("size: %d\n", ARRAY_SIZE);

      int val = addAll(1, 2, 3, 4, 5, 6, 7, 8);
      printf("val: %d\n", val);

      return 0;
    }
