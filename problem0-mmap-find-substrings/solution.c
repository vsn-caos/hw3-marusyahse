#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>


int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename> <search_string>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    const char *search_str = argv[2];
    size_t search_len = strlen(search_str);
    
    if (search_len == 0) {
        return 0;
    }

    // Открываем файл
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("fstat");
        close(fd);
        return 1;
    }

    if (st.st_size == 0) {
        close(fd);
        return 0;
    }

    char *data = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    close(fd);

    char *pos = data;
    char *end = data + st.st_size;
    
    while (pos <= end - search_len) {
        char *found = memmem(pos, end - pos, search_str, search_len);
        if (found == NULL) {
            break;
        }
        
        off_t offset = found - data;
        printf("%ld\n", (long)offset);
        
        pos = found + 1;
    }

    if (munmap(data, st.st_size) == -1) {
        perror("munmap");
        return 1;
    }

    return 0;
}
