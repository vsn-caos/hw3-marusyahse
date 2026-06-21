#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(void) {
    char expression[1024];
    
    if (fgets(expression, sizeof(expression), stdin) == NULL) {
        fprintf(stderr, "Failed to read expression\n");
        return 1;
    }
    
    size_t len = strlen(expression);
    if (len > 0 && expression[len - 1] == '\n') {
        expression[len - 1] = '\0';
    }
    
    char *args[] = {
        "python3",
        "-c",
        "print(",
        expression,
        ")",
        NULL
    };
    
    char command[2048];
    snprintf(command, sizeof(command), "print(%s)", expression);
    
    char *exec_args[] = {
        "python3",
        "-c",
        command,
        NULL
    };
    
    execvp("python3", exec_args);
    
    perror("execvp");
    return 1;
}
