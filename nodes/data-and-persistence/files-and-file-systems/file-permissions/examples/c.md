```c run id=umask-open-chmod
#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

static void show(const char *path) {
    struct stat st;
    stat(path, &st);
    char s[10] = "---------";
    const mode_t bits[9] = {S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP,
                            S_IXGRP, S_IROTH, S_IWOTH, S_IXOTH};
    for (int i = 0; i < 9; i++)
        if (st.st_mode & bits[i]) s[i] = "rwx"[i % 3];
    if (st.st_mode & S_ISUID) s[2] = (st.st_mode & S_IXUSR) ? 's' : 'S';
    printf("%-9s %04o %s\n", path, (unsigned)(st.st_mode & 07777), s);
}

int main(void) {
    // The umask removes bits from the mode argument of open and mkdir.
    mode_t old = umask(027);
    int fd = open("new.txt", O_WRONLY | O_CREAT | O_EXCL, 0666);
    close(fd);
    mkdir("dir", 0777);
    umask(old);
    show("new.txt");
    show("dir");

    // chmod sets the whole mode, with no umask.
    chmod("new.txt", 04755);
    show("new.txt");
    chmod("new.txt", 04644);
    show("new.txt");

    // access asks about the real user; no execute bit means no X_OK.
    printf("X_OK: %d\n", access("new.txt", X_OK));
    return 0;
}
```

```output for=umask-open-chmod
new.txt   0640 rw-r-----
dir       0750 rwxr-x---
new.txt   4755 rwsr-xr-x
new.txt   4644 rwSr--r--
X_OK: -1
```

- Verdict: library. ISO C has no permissions. POSIX gives `chmod`, `fchmod`, `umask` and the mode argument of `open` and `mkdir`, with the bit names of `<sys/stat.h>`. The umask applies to new files only, and `chmod` sets the mode as it is. `ls` writes a setuid bit as `s` when the owner can execute the file, and as `S` when the owner cannot.
