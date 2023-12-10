#include "arg_hand.h"
#include <stdlib.h>


int argument_handler(int argc, char* argv[], struct args* arg) {
  if (argc < REQ_ARG_COUNT || argc > REQ_ARG_COUNT)
    return ERROR;

  arg->n_physical = atoi(argv[1]);
  arg->page_size = atoi(argv[2]);
  arg->file_name = argv[3];

  return SUCCESS;
};
