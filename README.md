# 📂 OSP Assignment 3 - Memory Allocation
Built by Ryan Comerford (s3945787)

## 🚧 Building
- Compile all using `make all`.
- Compile only the first fit program using `make firstfit`.
- Compile only the best fit program using `make bestfit`.
- __Ensure c++ 20 features are enabled using `scl enable devtoolset-11 bash`.__

## 1️⃣ First Fit Allocation Strategy Program
``` bash
./firstfit [input_file]
```

- input_file: the file from which to read alloc/dealloc commands.

## 2️⃣ Best Fit Allocation Strategy Program
``` bash
./bestfit [input_file]
```

- input_file: the file from which to read alloc/dealloc commands.

## 🔴 Fatal Error Behavior

#### Main: Incorrect Format

- Cause: Program was run with incorrect/wrong number of arguments.
- Message: `Fatal Error: Program must be run in format ./[executable] [data_file].`

#### Loader: Unable to Open File

- Cause: Program could not open the given file (ie. does not exist / is locked).
- Message: `Fatal Error: Unable to open file "[data_file]".`

#### Loader: Unable to Process Line

- Cause: File given contained line which could not be parsed.
- Message: `Fatal Error: Unable to process input at line "[line]".`

#### Alloc: Incorrect Chunk Size

- Cause: Attempted to allocate memory in a chunk size larger than 512.
- Message: `Fatal Error: Chunk size ([chunk_size]) is too large.`

#### Dealloc: Unallocated Chunk

- Cause: Attempted to deallocate an address which does not exist or has already been deallocated.
- Message: `Fatal Error: Chunk "[chunk_address]" not found.`
