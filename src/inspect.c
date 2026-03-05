#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "../include/inspect.h"

// Read little-endian values
static uint64_t read_u64_le(FILE *f) {
    uint64_t v;
    if (fread(&v, sizeof(v), 1, f) != 1) {
        fprintf(stderr, "error: unexpected end of file (read_u64_le)\n");
        return 0;
    }
    return v;
}

static uint32_t read_u32_le(FILE *f) {
    uint32_t v;
    if (fread(&v, sizeof(v), 1, f) != 1) {
        fprintf(stderr, "error: unexpected end of file (read_u32_le)\n");
        return 0;
    }
    return v;
}

static void print_bytes(FILE *f, size_t n) {
    for (size_t i = 0; i < n; i++) {
        uint8_t b;
        if (fread(&b, 1, 1, f) != 1) {
            fprintf(stderr, "\nerror: unexpected end of file at byte %zu\n", i);
            return;
        }
        printf("%02x ", b);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    printf("\n");
}

void inspect_gguf(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        perror("fopen");
        return;
    }

    // Magic
    uint8_t magic[4];
    if (fread(magic, 1, 4, f) != 4) {
        fprintf(stderr, "error: could not read magic bytes\n");
        fclose(f);
        return;
    }
    printf("Magic   : %02x %02x %02x %02x  (\"%c%c%c%c\")\n",
           magic[0], magic[1], magic[2], magic[3],
           magic[0], magic[1], magic[2], magic[3]);

    if (memcmp(magic, "GGUF", 4) != 0) {
        printf("Not a GGUF file.\n");
        fclose(f);
        return;
    }

    // Version (u32)
    uint32_t version = read_u32_le(f);
    printf("Version : %u\n", version);

    // GGUF v2+ header: tensor_count (u64), kv_count (u64)
    uint64_t num_tensors = read_u64_le(f);
    uint64_t num_kv      = read_u64_le(f);

    printf("Tensors : %" PRIu64 "\n", num_tensors);
    printf("KV pairs: %" PRIu64 "\n", num_kv);

    printf("\nHex dump of next 256 bytes (start of metadata):\n");
    print_bytes(f, 256);

    fclose(f);
}
