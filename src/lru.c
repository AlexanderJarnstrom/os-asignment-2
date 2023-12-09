#include "arg_hand.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct mem_element {
  int last_ref;
  int val;
};

int lru(const struct args* arg);

int main(int argc, char *argv[])
{
  struct args arg;

  if (argument_handler(argc, argv, &arg) == ERROR) {
    printf("Wrong argument count expected %d, but got %d.\n", REQ_ARG_COUNT - 1, argc - 1);
    exit(1);
  }
  
  double j = 2;

  for (double i = 0; i < 8; i++) {
    arg.n_physical = pow(j,i);
    if (lru(&arg) == ERROR) {
      printf("Something went wrong.");
      exit(1);
    }
  }
  return SUCCESS;
}

int lru(const struct args* arg) {
  
  int cap, page_fault, total, p_size, size, exists, counter, value, i, oldest;
  char* file_name;
  char* line;
  size_t len;

  cap = arg->n_physical;
  p_size = arg->page_size;
  
  file_name = arg->file_name;
  len = 0;
  line = NULL;

  page_fault = 0;
  total = 0;
  size = 0;
  exists = 0;
  counter = 0;

  struct mem_element* mem_arr[cap];


  FILE* file = fopen(file_name, "r");
  if (file == NULL) {
    return ERROR; 
  }

  while(getline(&line, &len, file) != -1) { 
    value = atoi(line) / p_size;
    exists = 0;

    for (i = 0; i < size && !exists; i++) {
      if (mem_arr[i]->val == value) {
        exists = 1;
        mem_arr[i]->last_ref = counter;
      }
    }

    if (!exists && size != cap) {
      mem_arr[size] = (struct mem_element*)malloc(sizeof(struct mem_element));
      mem_arr[size]->val = value;
      mem_arr[size]->last_ref = counter;
      size++;
      page_fault++;
    } else if (!exists && size == cap) {
      oldest = 0;
      for (i = 0; i < cap; i++) {
        if (mem_arr[oldest]->last_ref > mem_arr[i]->last_ref) {
          oldest = i;
        }
      }

      mem_arr[oldest]->last_ref = counter;
      mem_arr[oldest]->val = value;
      page_fault++;
    }

    counter++;
    total++;
  }

  printf("Total %d | Page size: %d | Pages: %d\nPage faults: %d\n", total, p_size, cap, page_fault);

  fclose(file);
  return SUCCESS; 
}
