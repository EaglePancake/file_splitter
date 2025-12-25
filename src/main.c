#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define CHUNK_SIZE 1024

long parse_size(const char* str) {
    long num = 0;
    int i = 0;
    while (str[i] && isdigit(str[i])) {
        num = num * 10 + (str[i] - '0');
        i++;
    }
    if (str[i] == '\0') return num;
    // check suffix
    char suffix[3];
    int j = 0;
    while (str[i] && j < 2) {
        suffix[j++] = toupper(str[i++]);
    }
    suffix[j] = '\0';
    if (strcmp(suffix, "KB") == 0) return num * 1024;
    if (strcmp(suffix, "MB") == 0) return num * 1024 * 1024;
    if (strcmp(suffix, "GB") == 0) return num * 1024 * 1024 * 1024;
    return -1; // invalid
}

void split(const char* file, int mode, long value) {
    FILE* fp = fopen(file, "rb");

    if (!fp) return;

    fseek(fp, 0L, SEEK_END);
    long fileSize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    long partSize;
    long remainingSize;
    long parts;

    if (mode == 0) { // split by parts
        parts = value;
        partSize = fileSize / parts;
        remainingSize = fileSize % parts;
    } else { // split by size
        partSize = value;
        remainingSize = fileSize % partSize;
        parts = (fileSize + partSize - 1) / partSize;
    }

    printf("File size: %ld bytes\n", fileSize);
    if (mode == 0) {
        printf("Parts: %ld\n", value);
        printf("Each part size: %ld bytes\n", partSize);
        printf("Remaining size: %ld bytes\n", remainingSize);
        printf("Reconstructed size: %ld bytes\n", partSize * parts + remainingSize);
    } else {
        printf("Part size: %ld bytes\n", value);
        printf("Number of parts: %ld\n", parts);
        printf("Last part size: %ld bytes\n", remainingSize == 0 ? partSize : remainingSize);
    }
    
    for (int i = 0; i < parts; i++) {
        char partFileName[256];
        snprintf(partFileName, sizeof(partFileName), "%s.part%d", file, i + 1);
        FILE* partFp = fopen(partFileName, "wb");

        if (!partFp) {
            fclose(fp);
            return;
        }

        long bytesToWrite = (i == parts - 1) ? (mode == 0 ? partSize + remainingSize : (remainingSize == 0 ? partSize : remainingSize)) : partSize;
        char buffer[CHUNK_SIZE];
        long bytesWritten = 0;

        while (bytesWritten < bytesToWrite) {
            long toRead = (bytesToWrite - bytesWritten > CHUNK_SIZE) ? CHUNK_SIZE : (bytesToWrite - bytesWritten);
            long read = fread(buffer, 1, toRead, fp);
            fwrite(buffer, 1, read, partFp);
            bytesWritten += read;
        }

        fclose(partFp);
    }

    fclose(fp);
}

int main(int argc, char *argv[]) {
    
    if (argc != 4) {
        printf("Usage: %s <mode> <file> <value>\n", argv[0]);
        printf("Mode: p for parts, s for size\n");
        printf("For size mode, value can be in bytes or with KB/MB/GB suffix (e.g., 100MB)\n");
        return 1;
    }

    char* mode_str = argv[1];
    char* file = argv[2];
    long value;

    int mode;
    if (strcmp(mode_str, "p") == 0) {
        mode = 0;
        value = atol(argv[3]);
    } else if (strcmp(mode_str, "s") == 0) {
        mode = 1;
        value = parse_size(argv[3]);
        if (value == -1) {
            printf("Invalid size format. Use number with optional KB, MB, GB suffix.\n");
            return 1;
        }
    } else {
        printf("Invalid mode. Use 'p' for parts or 's' for size.\n");
        return 1;
    }

    printf("File: %s\n", file);
    if (mode == 0) {
        printf("Parts: %ld\n", value);
    } else {
        printf("Part size: %ld bytes\n", value);
    }

    split(file, mode, value);

    return 0;
}