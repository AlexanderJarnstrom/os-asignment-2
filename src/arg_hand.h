#ifndef ARG_HAND
#define ARG_HAND

#define REQ_ARG_COUNT 4
#define ERROR -1
#define SUCCESS 0


/**
 * The struct "args" represents a set of arguments including the number of physical pages, page size,
 * and file name.
 * @property {unsigned int} n_physical - The number of physical pages in the memory.
 * @property {unsigned int} page_size - The `page_size` property represents the size of a page in a
 * memory system. It is typically measured in bytes and determines the amount of data that can be
 * stored in a single page of memory.
 * @property {char} file_name - The file_name property is a pointer to a character array, which
 * represents the name of a file.
 */
struct args {
  unsigned int n_physical;
  unsigned int page_size;
  char* file_name;
};

/**
 * The function `argument_handler` takes command line arguments and assigns them to the corresponding
 * fields in a struct.
 * 
 * @param argc The argc parameter is an integer that represents the number of command-line arguments
 * passed to the program. It includes the name of the program itself as the first argument.
 * @param argv argv is an array of strings that contains the command-line arguments passed to the
 * program. Each element of the array represents a separate argument, with the first element (argv[0])
 * being the name of the program itself.
 * @param arg The parameter `arg` is a pointer to a struct `args`.
 * 
 * @return either the value of ERROR or SUCCESS.
 */
int argument_handler(int argc, char* argv[], struct args* arg);

#endif // !ARG_HAND
