#ifndef ARG_HAND
#define ARG_HAND

#define REQ_ARG_COUNT 4
#define ERROR -1
#define SUCCESS 0


struct args {
  unsigned int n_physical;
  unsigned int page_size;
  char* file_name;
};

int argument_handler(int argc, char* argv[], struct args* arg);

#endif // !ARG_HAND
