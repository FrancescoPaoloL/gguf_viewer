#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

// Read little-endian values
static uint64_t read_u64_le(FILE *f) {
    uint64_t v;
    fread(&v, sizeof(v), 1, f);
    return v;
}

static uint32_t read_u32_le(FILE *f) {
    uint32_t v;
    fread(&v, sizeof(v), 1, f);
    return v;
}

static void print_bytes(FILE *f, size_t n) {
    for (size_t i = 0; i < n; i++) {
        uint8_t b;
        fread(&b, 1, 1, f);
        printf("%02x ", b);
        if ((i+1)%16==0) printf("\n");
    }
    printf("\n");
}

void inspect_gguf(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        perror("fopen");
        return;
    }

    uint8_t magic[4];
    fread(magic, 1, 4, f);
    printf("Magic bytes: %02x %02x %02x %02x  -> \"%c%c%c%c\"\n",
           magic[0], magic[1], magic[2], magic[3],
           magic[0], magic[1], magic[2], magic[3]);

    if (memcmp(magic, "GGUF", 4) != 0) {
        printf("Not a GGUF file.\n");
        fclose(f);
        return;
    }

    uint32_t version;
    fread(&version, sizeof(version), 1, f);
    printf("Version (raw little-endian): %u\n", version);

    uint64_t num_kv = read_u64_le(f);
    uint64_t num_tensors = read_u64_le(f);
    uint64_t _reserved = read_u64_le(f);

    printf("Next 3 u64 values (little-endian):\n");
    printf("  num_kv: %" PRIu64 " (0x%016" PRIx64 ")\n", num_kv, num_kv);
    printf("  num_tensors: %" PRIu64 " (0x%016" PRIx64 ")\n", num_tensors, num_tensors);
    printf("  reserved: %" PRIu64 " (0x%016" PRIx64 ")\n", _reserved, _reserved);

    printf("\nHex dump of the next 256 bytes (offset after header):\n");
    print_bytes(f, 256);

    fclose(f);
}

