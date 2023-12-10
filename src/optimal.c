#include "arg_hand.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#define MEM_REF_SIZE 100000

/**
 * The struct mem_element represents an element in a memory structure with a value and a reference to
 * the next element.
 * @property {int} val - The "val" property is an integer that represents the value stored in the
 * memory element.
 * @property {int} next_ref - The "next_ref" property in the "mem_element" struct is an integer that
 * represents the index to the next element.
 */
struct mem_element {
  int val;
  int next_ref;
};

/**
 * The struct "file" represents a file with its name, content, length, and stream.
 * @property {char} name - A pointer to a character array that represents the name of the file.
 * @property {char} line - The "line" property is a pointer to a character array that represents a line
 * of text in a file.
 * @property {size_t} len - The `len` property is of type `size_t` and represents the length of the
 * `line` property. It is used to store the number of characters in the line.
 * @property {FILE} stream - The `stream` property is a pointer to a `FILE` structure, which represents
 * a file stream. It is used for reading from a file.
 */
struct file {
  char* name;
  char* line;
  size_t len;
  FILE* stream;
};

/**
 * The struct mem_element_arr is a data structure that represents an array of mem_element objects with
 * a specified size and capacity.
 * @property arr - The "arr" property is a pointer to an array of struct mem_element elements.
 * @property {int} size - The size property represents the current number of elements in the array.
 * @property {int} cap - The "cap" property represents the capacity of the dynamic array. It indicates
 * the maximum number of elements that the array can currently hold.
 */
struct mem_element_arr {
  struct mem_element* arr;
  int size;
  int cap;
};

int optimal(const struct args* arg);
int get_mem_refs(struct mem_element_arr* arr, struct file* file, const struct args* arg);

void update_next_ref_loop(struct mem_element_arr* arr);
void update_next_ref_val(struct mem_element* current, struct mem_element* next, int *found, int next_ref);
void iterate_arr (struct mem_element_arr* arr, struct mem_element_arr* added_arr);

int exists(int next_ref, const int value, struct mem_element_arr* added_arr);
int latest_ref_index(int start, struct mem_element_arr* added_arr);

int main(int argc, char *argv[])
{
  struct args arg;
  
  if (argument_handler(argc, argv, &arg) == ERROR) {
    printf("Wrong argument count expected %d, but got %d.\n", REQ_ARG_COUNT - 1, argc - 1);
    exit(1);
  }
  
  if (optimal(&arg) == ERROR) {
    printf("Something went wrong.\n");
    exit(1);
  }  
  
  return SUCCESS;
}

/**
 * The function "optimal" takes in a struct of arguments, reads memory references from a file, and
 * performs an optimal page replacement algorithm.
 * 
 * @param arg A pointer to a struct args, which contains various parameters for the optimal function.
 * 
 * @return either SUCCESS or ERROR.
 */
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

  file.name = arg->file_name;
  file.len = 0;
  file.line = NULL;
  file.stream = fopen(file.name, "r");
  if (file.stream == NULL) return ERROR;

  if (get_mem_refs(&arr, &file, arg) == ERROR) return ERROR;

  fclose(file.stream);

  update_next_ref_loop(&arr);

  printf("Pages: %d | Page size: %d\n", arg->n_physical, arg->page_size);
  iterate_arr(&arr, &added_arr);

  return SUCCESS;
}


/**
 * The function reads lines from a file, converts them to integers, divides them by a page size, and
 * stores them in an array.
 * 
 * @param arr A pointer to a struct mem_element_arr, which contains an array of mem_element structs.
 * @param file The "file" parameter is a pointer to a struct called "file".
 * @param arg The "arg" parameter is a pointer to a struct of type "args".
 * 
 * @return either SUCCESS or ERROR.
 */
int get_mem_refs(struct mem_element_arr* arr, struct file* file, const struct args* arg) { 
  while (getline(&file->line, &file->len, file->stream) != -1) {
    if (arr->size >= arr->cap) return ERROR;

    arr->arr[arr->size].val = atoi(file->line) / arg->page_size;
    arr->size++;
  }

  return SUCCESS;
}

/**
 * The function updates the next reference value for each element in an array by comparing it with the
 * elements that come after it.
 * 
 * @param arr The parameter "arr" is a pointer to a struct called "mem_element_arr".
 */
void update_next_ref_loop(struct mem_element_arr* arr) {
  int i, j, found;

  for (i = 0; i < arr->size; i++) {
    found = 0;
    for (j = i + 1; j < arr->size && !found; j++)
      update_next_ref_val(&arr->arr[i], &arr->arr[j], &found, j);
  }
}

/**
 * The function updates the next reference value of a current memory element based on the value of the
 * next memory element, and sets a flag if a match is found.
 * 
 * @param current A pointer to a struct mem_element representing the current element.
 * @param next The "next" parameter is a pointer to a struct mem_element, which represents the next
 * refrenced element.
 * @param found The "found" parameter is a pointer to an integer variable that is used to indicate
 * whether a match has been found between the values of the current and next elements. It is initially
 * set to 0 and will be updated to 1 if a match is found.
 * @param next_ref The parameter `next_ref` is an integer that represents the index of the
 * next element of the same value.
 */
void update_next_ref_val(struct mem_element* current, struct mem_element* next, int *found, const int next_ref) {
  if (current->val == next->val) {
    current->next_ref = next_ref;
    *found = 1;
  } else if (!*found)
    current->next_ref = -1;
}

/**
 * The function "iterate_arr" iterates through an array, checks if each element exists in another
 * array, and updates the second array accordingly while keeping track of page faults and total
 * iterations.
 * 
 * @param arr The parameter `arr` is a pointer to a struct `mem_element_arr`.
 * @param added_arr The parameter `added_arr` is a pointer to a struct `mem_element_arr`.
 */
void iterate_arr (struct mem_element_arr* arr, struct mem_element_arr* added_arr) {
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
}

/**
 * The function "exists" checks if a given value exists in a given array and updates the next reference
 * if it does.
 * 
 * @param next_ref The parameter `next_ref` is an integer that represents the next reference value.
 * @param value The "value" parameter is an integer value that we are checking for existence in the
 * "added_arr" array.
 * @param added_arr The parameter `added_arr` is a pointer to a struct `mem_element_arr`.
 * 
 * @return an integer value. If the value is 1, it means that the element with the given value exists
 * in the "added_arr" array and the "next_ref" value has been updated for that element. If the value is
 * 0, it means that the element with the given value does not exist in the "added_arr" array.
 */
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

/**
 * The function `latest_ref_index` returns the index of the element in the `added_arr` array with the
 * latest `next_ref` value.
 * 
 * @param start The "start" parameter is an integer representing the starting index for the iteration
 * in the for loop. It determines the index from which the loop will start iterating over the elements
 * in the "added_arr" array.
 * @param added_arr The parameter `added_arr` is a pointer to a struct `mem_element_arr`.
 * 
 * @return the index of the latest element in the `added_arr` array that has a `next_ref` value of -1
 * or the highest `next_ref` value among all elements in the array.
 */
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
