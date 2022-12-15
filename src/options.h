#include <stdbool.h>

enum Input {RDRAND, MRAND48_R, SLASH_F };
enum Output {STDIO, N};

struct opts {
    bool valid; 
    long long nbytes;
    enum Input input;
    char* r_src;
    enum Output output;
    unsigned int block_size; 
};

void read_options(int argc, char *argv[], struct opts* opts);