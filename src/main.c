#include <stdio.h>

// forward declaration
void inspect_gguf(const char *filename);

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s model.gguf\n", argv[0]);
        return 1;
    }

    inspect_gguf(argv[1]);
    return 0;
}

