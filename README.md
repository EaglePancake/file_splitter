# File Splitter

A simple command-line utility written in C for splitting large files into smaller parts. Supports splitting by number of parts or by specified part size.

## Features

- Split files into a specified number of equal parts
- Split files into parts of a specified size (in bytes)
- Cross-platform compatibility (Windows/Linux/macOS)

## Requirements

- C compiler (GCC, Clang, MSVC)
- CMake (version 3.10 or higher)

## Building

1. Clone or download the project
2. Create a build directory:
   ```bash
   mkdir build
   cd build
   ```

3. Generate build files with CMake:
   ```bash
   cmake ..
   ```

4. Build the project:
   ```bash
   cmake --build . --config Release
   ```

   On Windows with Visual Studio:
   ```bash
   cmake --build . --config Release
   ```

## Usage

The compiled executable is named `file_splitter.exe` (Windows) or `file_splitter` (Linux/macOS).

### Basic Syntax
```
file_splitter <mode> <filename> <value>
```

### Modes

#### Split by Number of Parts (`p`)
Splits the file into a specified number of roughly equal parts.

```bash
file_splitter p input_file.txt 5
```

This creates 5 files: `input_file.txt.part1`, `input_file.txt.part2`, etc.

#### Split by Part Size (`s`)
Splits the file into parts of a specified size in bytes. You can specify the size with optional suffixes: KB, MB, GB (case-insensitive).

```bash
file_splitter s input_file.txt 1MB
```

This creates parts of 1MB each. The last part may be smaller if the file size doesn't divide evenly.

Supported formats:
- `1048` (bytes)
- `1KB` (1024 bytes)
- `1MB` (1,048,576 bytes)  
- `1GB` (1,073,741,824 bytes)

### Examples

```bash
# Split a large video file into 10 parts
file_splitter p movie.mp4 10

# Split a log file into 100MB chunks
file_splitter s application.log 100MB

# Split a backup file into 1GB parts
file_splitter s backup.tar 1GB
```

## Output

The program provides detailed information about the splitting process:

- Original file size
- Number of parts or part size
- Size of each part
- Size of the last part (if different)

## Notes

- The original file remains unchanged
- Part files are created in the same directory as the original file
- Part files are named with `.part{N}` suffix where N is the part number starting from 1

## Limitations

- File paths with spaces may need to be quoted
- Very large files (>2GB) may have issues on 32-bit systems due to integer overflow
- No progress indicator for large files

## License

This project is open source. Feel free to use and modify as needed.