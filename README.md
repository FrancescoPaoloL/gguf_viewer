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
* **num_tensors:** number of tensors stored in the model.
* **num_kv:** number of key-value entries (metadata entries).

> Note: GGUF v2+ has no `reserved` field. The original tool was reading a phantom third `u64`.

Example output from `inspect`:
```
Magic   : 47 47 55 46  ("GGUF")
Version : 3
Tensors : 26
KV pairs: 291
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

The `inspect` tool prints the first 512 bytes (`HEX_DUMP_BYTES`) of the KV section as a hex dump with ASCII column. Example keys:
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
    curl -L -o models/qwen2.5-0.5b-instruct-q4_0.gguf \
    "https://huggingface.co/Qwen/Qwen2.5-0.5B-Instruct-GGUF/resolve/main/qwen2.5-0.5b-instruct-q4_0.gguf"
### Compile and inspect a GGUF model
```bash
make inspect MODEL=models/qwen2.5-0.5b-instruct-q4_0.gguf
```
This prints:
* Magic bytes
* Version
* Number of tensors
* Number of KV entries
* First 512 bytes of metadata as hex dump with ASCII column
