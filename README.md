# GGUF Viewer (Inspect Only)

This tool provides a simple way to inspect the header and metadata of GGUF (General Graph Unified Format) model files.

It **does not yet parse tensor data**, only the header and the first part of the key-value metadata.

## File Structure

Any GGUF file has the following high-level structure:
```
[Header][KV Pairs / Metadata][Tensors / Data]
```
Specifically:

### 1. Header

The header contains general information about the model:

* **Magic bytes:** `GGUF` — identifies the file type.
* **Version:** current GGUF version (e.g. 3).
* **num_kv:** number of key-value entries (metadata entries).
* **num_tensors:** number of tensors stored in the model.
* **reserved:** additional header space (currently not used in inspect).

Example output from `inspect`:
```
Magic bytes: GGUF
Version: 3
num_kv: 291
num_tensors: 26
reserved: 20
```
### 2. Key-Value Pairs / Metadata

Each KV entry contains:

* **Key:** string, e.g., `"general.architecture"`.
* **Type:** integer code representing the value type:

  * `0` → int
  * `1` → float
  * `8` → string
  * others → blob / compound

* **Value:** primitive type (int, float, string) or compound (array of entries).

The `inspect` tool prints the first 256 bytes of the KV section as a hex dump. Example keys:
```
general.architecture -> "qwen2"
general.type         -> "model"
general.name         -> "qwen2.5-0.5b-instruct"
general.version      -> "v0.1"
general.finetune     -> "qwen2.5-0.5b-instruct"
general.size_label   -> "630M"
```

> Nested or tensor values are not parsed; they appear as blobs in the hex dump.

### 3. Tensors / Data

* Tensor data is stored as raw bytes.
* Each tensor usually contains:

  * **Name** (string)
  * **Shape** (array of integers)
  * **Type** (float32, float16, int8, etc.)
  * **Data** (raw bytes)

> The `inspect` tool currently does not decode these tensors.

---

## How to use

### Download a GGUF model (just an example)

    curl -L -o qwen2.5-0.5b-instruct-q4_0.gguf \
    "https://huggingface.co/Qwen/Qwen2.5-0.5B-Instruct-GGUF/resolve/main/qwen2.5-0.5b-instruct-q4_0.gguf"

### Compile and inspect a GGUF model
```bash
make inspect MODEL=models/qwen2.5-0.5b-instruct-q4_0.gguf
```
This prints:

* Magic bytes
* Version
* Number of KV entries
* Number of tensors
* First 256 bytes of metadata

