```c wrong id=system
#include <stdio.h>
#include <stdlib.h>

static void look_up(const char *name) {
    char command[256];
    snprintf(command, sizeof command, "echo looking up %s", name);
    fflush(stdout);
    system(command);
}

int main(void) {
    look_up("report.txt");
    look_up("report.txt; echo INJECTED");
    return 0;
}
```

```output for=system
looking up report.txt
looking up report.txt
INJECTED
```

```c run id=posix-spawn
#define _POSIX_C_SOURCE 200809L
#include <spawn.h>
#include <stdio.h>
#include <sys/wait.h>

extern char **environ;

static int look_up(const char *name) {
    char *argv[] = {"echo", "looking up", (char *)name, NULL};
    pid_t pid;
    fflush(stdout);
    int err = posix_spawnp(&pid, "echo", NULL, NULL, argv, environ);
    if (err != 0) {
        return err;
    }
    int status;
    waitpid(pid, &status, 0);
    return 0;
}

int main(void) {
    look_up("report.txt");
    look_up("report.txt; echo INJECTED");
    return 0;
}
```

```output for=posix-spawn
looking up report.txt
looking up report.txt; echo INJECTED
```

- Verdict: stdlib. ISO C has only `system`, which passes the string to a command processor, `/bin/sh -c` on POSIX systems. POSIX adds `posix_spawn`, `posix_spawnp` and the `exec` family, which take the program and an array of arguments and start no shell.
- `popen` also runs its command through the shell. The phf program of 1996 built a command line with `strcat` and passed it to `popen`.
- `snprintf` limits the length of the command, so the first example has no buffer overflow. It does nothing against the `;`.
- The `fflush(stdout)` calls keep the output of the parent and the child in order, because the child writes to the same file descriptor.
