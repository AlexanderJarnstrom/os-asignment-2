
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

#include "arg_hand.h"

int fifo(struct args* arg);

int main(int argc, char *argv[])
{
  struct args arg;
  
  if (argument_handler(argc, argv, &arg) == ERROR) {
    printf("Wrong argument count expected %d, but got %d.\n", REQ_ARG_COUNT - 1, argc - 1);
    exit(1);
  }
  
  if (fifo(&arg) == ERROR) {
    printf("Something went wrong.\n");
    exit(1);
  }
  
  return SUCCESS;
}

/**
 * The function implements the FIFO (First-In-First-Out) page replacement algorithm.
 * 
 * @param arg The "arg" parameter is a pointer to a struct called "args".
 * 
 * @return The code does not explicitly return a value.
 */
int fifo(struct args* arg) {
  
  int cap, page_size, size, oldest_ptr, youngest_ptr, value;
  int page_fault, total, i, exists;

  size = 0;
  oldest_ptr = 0;
  youngest_ptr = 0;
  cap = arg->n_physical;
  page_size = arg->page_size;
  page_fault = 0;
  total = 0;

  int arr[cap];

  FILE* file = fopen(arg->file_name, "r");
  if (file == NULL) {
    return ERROR;
  }
  
  char* line = NULL;
  size_t len = 0;

  for (i = 0; i < cap; i++) {
    arr[i] = 0;
  }

  while(getline(&line, &len, file) != -1) {
    value = atoi(line) / page_size * page_size;
    exists = 0;

    for (i = 0; i < size && exists != 1; i++)
      if (arr[i] ==  value) exists = 1;

    if (size == 0) {
      arr[youngest_ptr] = value;
      size++;
      page_fault++;

    } else if (!exists && size != cap) {
      youngest_ptr = (youngest_ptr + 1) % cap;
      size++;
      arr[youngest_ptr] = value;
      page_fault++;
    } else if (!exists && size == cap) {
      arr[oldest_ptr] = value;
      oldest_ptr = (oldest_ptr + 1) % cap;
      youngest_ptr = (oldest_ptr + 1) % cap;
      page_fault++;
    }

    total++;
  }

  printf("Page size: %d | Page count: %d\n", arg->page_size, arg->n_physical);
  printf("Page faults: %d | Total: %d\n", page_fault, total);
  
  fclose(file);

  return SUCCESS;
}

