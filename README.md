# DeepCache Chatbot

DeepCache is a high-performance C++ inference router that combines a custom BPE-style tokenizer with a semantic response cache. Repeated or closely related prompts can be answered locally, avoiding an expensive model call.

## Architecture

- `tokenizer.hpp`: normalizes whitespace-delimited text and assigns stable integer IDs to known and new words.
- `semantic_cache.hpp`: compares unique token signatures with a Jaccard similarity index and returns responses above a 0.65 threshold.
- `main.cpp`: seeds a response, runs exact, similar, and unrelated queries, and simulates an 800 ms cloud inference on misses.
- `CMakeLists.txt`: configures a C++17 executable named `DeepCacheEngine`.

## Build and run

```bash
cmake -S . -B build
cmake --build build
./build/DeepCacheEngine
```

The first two queries demonstrate cache hits. The unrelated query demonstrates the fallback path and is intentionally delayed to represent a remote inference call.