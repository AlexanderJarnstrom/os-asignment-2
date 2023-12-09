#include "src/arg_hand.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define MEM_REF_SIZE 100000

struct mem_element {
  int val;
  int next_ref;
};

struct file {
  char* name;
  char* line;
  size_t len;
  FILE* stream;
};

struct mem_element_arr {
  struct mem_element* arr;
  int size;
  int cap;
};

int optimal(const struct args* arg);
int get_mem_refs(struct mem_element_arr* arr, struct file* file, const struct args* arg);
int update_next_ref(struct mem_element_arr* arr);
int iterate_arr (struct mem_element_arr* arr, struct mem_element_arr* added_arr);
int exists(int next_ref, const int value, struct mem_element_arr* added_arr);
int latest_ref_index(int start, struct mem_element_arr* added_arr);

int main(int argc, char *argv[])
{
  struct args arg;
  /*
  if (argument_handler(argc, argv, &arg) == ERROR) {
    printf("Wrong argument count expected %d, but got %d.\n", REQ_ARG_COUNT - 1, argc - 1);
    exit(1);
  }
  */
  double j = 2;
  
  for (double x = 7; x < 11; x++) {
    printf("---------------------------------------\n");
    arg.page_size = pow(j, x);
    for (double i = 0; i < 8; i++) {
      arg.n_physical = pow(j, i);
      if (optimal(&arg) == ERROR) {
        exit(1);
      }
    }
  }
  
  
  return SUCCESS;
}

int optimal(const struct args* arg) {

  struct file file;
  struct mem_element_arr arr;
  struct mem_element_arr added_arr;
 
  arr.size = 0;
  arr.cap = MEM_REF_SIZE;
  arr.arr = malloc(sizeof(struct mem_element) * arr.cap);

  added_arr.size = 0;
  added_arr.cap = arg->n_physical;
  added_arr.arr = malloc(sizeof(struct mem_element) * added_arr.cap);

  // Open file stream and init file variables.
  file.name = arg->file_name;
  file.len = 0;
  file.line = NULL;
  file.stream = fopen(file.name, "r");
  if (file.stream == NULL) return ERROR;

  // Get content from file.
  if (get_mem_refs(&arr, &file, arg) == ERROR) return ERROR;
  // File no longer needed.
  fclose(file.stream);

  // Update next refrence.
  if (update_next_ref(&arr) == ERROR) return ERROR;

  printf("Pages: %d | Page size: %d\n", arg->n_physical, arg->page_size);
  if (iterate_arr(&arr, &added_arr) == ERROR) return ERROR;

  return SUCCESS;
}

int get_mem_refs(struct mem_element_arr* arr, struct file* file, const struct args* arg) { 
  while (getline(&file->line, &file->len, file->stream) != -1) {
    if (arr->size >= arr->cap) return ERROR;

    arr->arr[arr->size].val = atoi(file->line) / arg->page_size;
    arr->size++;
  }

  return SUCCESS;
}

int update_next_ref(struct mem_element_arr* arr) {
  int i, j, found;

  for (i = 0; i < arr->size; i++) {
    found = 0;
    for (j = i + 1; j < arr->size && !found; j++) {
      if (arr->arr[i].val == arr->arr[j].val) {
        arr->arr[i].next_ref = j;
        found = 1;
      } else if (!found) {
        arr->arr[i].next_ref = -1;
      }
    }
  }

  return SUCCESS;
}

int iterate_arr (struct mem_element_arr* arr, struct mem_element_arr* added_arr) {
  int i, j, val_exists, latest, page_faults, total;
  page_faults = 0;
  total = 0;

  for (i = 0; i < arr->size; i++) {
    val_exists = exists(arr->arr[i].next_ref, arr->arr[i].val, added_arr);

    if (!val_exists && added_arr->size != added_arr->cap) {
      added_arr->arr[added_arr->size] = arr->arr[i];
      added_arr->size++;
      page_faults++;
    } else if (!val_exists && added_arr->size == added_arr->cap) {
      latest = latest_ref_index(i, added_arr);
      added_arr->arr[latest] = arr->arr[i];
      page_faults++;
    }

    total++;
  } 

  printf("Page faults: %d | Total: %d\n", page_faults, total);

  return SUCCESS;
}

int exists(const int next_ref, const int value, struct mem_element_arr* added_arr) {
  int i;
  

  for (i = 0; i < added_arr->size; i++) {
    if (added_arr->arr[i].val == value) {
      added_arr->arr[i].next_ref = next_ref;
      return 1;
    }  
  }

  return 0;
}

int latest_ref_index(int start, struct mem_element_arr* added_arr) {
  int i, latest;
  latest = 0;

  for (i = 1; i < added_arr->size && added_arr->arr[latest].next_ref != -1; i++) {   
    if (added_arr->arr[i].next_ref == -1) {
      latest = i;
    } else if (added_arr->arr[i].next_ref > added_arr->arr[latest].next_ref) {
      latest = i;
    }
  }

  return latest;
}
