```c run id=pipe-to-child-process
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

/* C has no message passing in the language. POSIX gives processes pipes,
   which carry bytes from one process to another. */
int main(void) {
    int to_child[2];
    int to_parent[2];
    if (pipe(to_child) != 0 || pipe(to_parent) != 0) {
        return 1;
    }
    int counter = 10;
    pid_t pid = fork();
    if (pid < 0) {
        return 1;
    }
    if (pid == 0) {
        /* The child has its own copy of the memory, so this does not change the parent. */
        counter = 99;
        close(to_child[1]);
        close(to_parent[0]);
        int n;
        int sum = 0;
        while (read(to_child[0], &n, sizeof n) == (ssize_t)sizeof n) {
            sum += n;
        }
        write(to_parent[1], &sum, sizeof sum);
        write(to_parent[1], &counter, sizeof counter);
        _exit(0);
    }
    close(to_child[0]);
    close(to_parent[1]);
    for (int n = 1; n <= 5; n++) {
        write(to_child[1], &n, sizeof n);
    }
    /* Closing the write end tells the child that no more messages come. */
    close(to_child[1]);
    int sum = 0;
    int child_counter = 0;
    read(to_parent[0], &sum, sizeof sum);
    read(to_parent[0], &child_counter, sizeof child_counter);
    waitpid(pid, NULL, 0);
    printf("sum %d\n", sum);
    printf("child counter %d, parent counter %d\n", child_counter, counter);
    return 0;
}
```

```output for=pipe-to-child-process
sum 15
child counter 99, parent counter 10
```

- Verdict: idiom. The language has no message passing, so the example uses POSIX `pipe` and `fork`. The parent writes five integers, closes the pipe to mark the end, and reads the sum from a second pipe. The child sets `counter` to 99 in its own copy of the memory, so the parent still has 10.
