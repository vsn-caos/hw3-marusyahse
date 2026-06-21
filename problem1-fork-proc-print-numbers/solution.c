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
            printf("%d", i + 1);
            if (i + 1 < n) {
                printf(" ");
            } else {
                printf("\n");
            }
            fflush(stdout);
            return 0;
        } else {
            // Родительский процесс
            printf("%d", i);
            if (i < n) {
                printf(" ");
            }
            fflush(stdout);
            wait(NULL);
            // Не завершаемся сразу, а продолжаем цикл
            // Но только для первого процесса
            if (i > 1) {
                return 0;
            }
        }
    }
    
    return 0;
}
