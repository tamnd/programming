```cpp run id=child
#include <cstdio>
#include <print>
#include <sys/wait.h>
#include <unistd.h>

int counter = 0;

int main() {
    std::fflush(stdout);
    pid_t pid = fork();
    if (pid == -1) {
        return 1;
    }
    if (pid == 0) {
        counter += 100;
        std::println("child: counter = {}", counter);
        std::fflush(stdout);
        _exit(3);
    }
    int status = 0;
    waitpid(pid, &status, 0);
    std::println("child exit status: {}", WIFEXITED(status) ? WEXITSTATUS(status) : -1);
    std::println("child pid differs: {}", pid != getpid());
    std::println("parent: counter = {}", counter);
}
```

```output for=child
child: counter = 100
child exit status: 3
child pid differs: true
parent: counter = 0
```

- Verdict: stdlib through POSIX. The example calls `fork` and `waitpid` from the POSIX C API.
- `fork` returns twice: 0 in the child and the ID of the child in the parent. The child changes its own copy of `counter`.
- The child ends with `_exit`, which does not flush the buffers of `stdout`, so the child flushes them first. The parent also flushes before `fork`, so the child does not inherit and print a copy of buffered output.
- `waitpid` waits for the child and collects its status, so no zombie stays. `WEXITSTATUS` takes the exit code out of the status.
