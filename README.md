# guff_viewer

GGUF viewer

1. GGUF File Structure Overview

GGUF (General Graph Unified Format) is designed to store models in a structured, binary way. Its structure is roughly:

[Header] [KV Pairs  / Metadata]


Header: contains general information about the model.

Magic bytes: identifies the file type

Version: GGUF version

Number of key-value entries

Offsets: sometimes pointers to data sections

Key-Value Pairs / Metadata:

Each entry has:

Key: string

Type: one of int, float, string, compound

Value: either primitive or a compound (array/struct)

Tensors / Data:

Tensor data is stored as arrays (floats, ints, quantized ints, etc.)

Each tensor may have:

Name (string)

Shape (array of ints)

Type (float32, float16, int8, etc.)

Data (raw bytes)


## Build

make inspect MODEL=../llama.cpp/models/qwen2.5-0.5b-instruct-q4_0.gguf
make valgrind MODEL=../llama.cpp/models/qwen2.5-0.5b-instruct-q4_0.gguf


```
make
```

## Run

```
./bin/app
```

