#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <N>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        fprintf(stderr, "N must be positive\n");
        return 1;
    }

    // Для N=1 просто выводим "1\n"
    if (n == 1) {
        printf("1\n");
        return 0;
    }

    for (int i = 1; i < n; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            return 1;
        }
        if (pid == 0) {
            // Дочерний процесс
            if (i + 1 == n) {
                // Последний процесс выводит число и перевод строки
                printf("%d\n", i + 1);
            } else {
                // Остальные процессы выводят число и пробел
                printf("%d ", i + 1);
            }
            return 0;
        } else {
            // Родительский процесс
            printf("%d ", i);
            fflush(stdout);
            wait(NULL);
            return 0;
        }
    }
    
    return 0;
}
