```c run id=readlink-has-no-nul
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void) {
    FILE *f = fopen("data.txt", "w");
    if (f == NULL || fputs("alpha\n", f) < 0 || fclose(f) != 0) return 1;
    if (link("data.txt", "hard.txt") != 0 || symlink("data.txt", "soft.txt") != 0) return 1;

    struct stat st, lst;
    if (stat("soft.txt", &st) != 0 || lstat("soft.txt", &lst) != 0) return 1;
    printf("stat:  regular %d, nlink %ld\n", S_ISREG(st.st_mode), (long)st.st_nlink);
    printf("lstat: link %d, size %ld\n", S_ISLNK(lst.st_mode), (long)lst.st_size);

    // readlink does not add a NUL byte, so the code adds it.
    char buf[64];
    ssize_t n = readlink("soft.txt", buf, sizeof buf - 1);
    if (n < 0) return 1;
    buf[n] = '\0';
    printf("bytes: %zd\n", n);
    printf("target: %s\n", buf);
    return 0;
}
```

```output for=readlink-has-no-nul
stat:  regular 1, nlink 2
lstat: link 1, size 8
bytes: 8
target: data.txt
```

- Verdict: library. ISO C has no links. POSIX gives `link`, `symlink`, `readlink`, `lstat` and `unlink`. The size of a symbolic link in `lstat` is the length of its target, and `readlink` does not end the target with a NUL byte.
