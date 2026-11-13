```c run id=stat-and-lstat
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

static const char *kind(mode_t m) {
    if (S_ISREG(m)) return "regular file";
    if (S_ISDIR(m)) return "directory";
    if (S_ISLNK(m)) return "symbolic link";
    return "other";
}

int main(void) {
    FILE *f = fopen("data.txt", "w");
    fputs("hello", f);
    fclose(f);
    chmod("data.txt", 0640);
    link("data.txt", "second-name.txt");
    symlink("data.txt", "link");

    struct stat st, lst, other;
    // stat follows the link, lstat does not.
    if (stat("link", &st) != 0 || lstat("link", &lst) != 0) {
        perror("stat");
        return 1;
    }
    printf("stat:  %s, size %lld, mode %03o, links %lu\n", kind(st.st_mode),
           (long long)st.st_size, (unsigned)(st.st_mode & 0777),
           (unsigned long)st.st_nlink);
    printf("lstat: %s, size %lld\n", kind(lst.st_mode), (long long)lst.st_size);
    printf("owner is me: %d\n", st.st_uid == geteuid());

    stat("second-name.txt", &other);
    printf("same file: %d\n", st.st_dev == other.st_dev && st.st_ino == other.st_ino);

    // A dangling link: stat fails, lstat works.
    symlink("missing.txt", "dangling");
    printf("stat dangling: %d, lstat dangling: %d\n",
           stat("dangling", &st), lstat("dangling", &lst));
    return 0;
}
```

```output for=stat-and-lstat
stat:  regular file, size 5, mode 640, links 2
lstat: symbolic link, size 8
owner is me: 1
same file: 1
stat dangling: -1, lstat dangling: 0
```

- Verdict: library. ISO C has no call that reads the metadata of a file. POSIX `stat` fills a `struct stat` from `<sys/stat.h>`, and `lstat` does the same for a link without following it. The macros `S_ISREG`, `S_ISDIR` and `S_ISLNK` read the file type from `st_mode`, and the low nine bits of `st_mode` are the permission bits. The pair `st_dev` and `st_ino` identifies the file.
