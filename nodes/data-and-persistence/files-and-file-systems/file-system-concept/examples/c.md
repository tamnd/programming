```c run id=posix-stat
#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <unistd.h>

static void put(const char *path, const char *text) {
    FILE *f = fopen(path, "w");
    fputs(text, f);
    fclose(f);
}

int main(void) {
    mkdir("tree", 0755);
    put("tree/readme.txt", "read me\n");

    // stat fills a struct stat: the type is in st_mode, the identity in st_dev and st_ino.
    struct stat a, b;
    stat("tree", &a);
    printf("tree is a directory: %d\n", S_ISDIR(a.st_mode));
    stat("tree/readme.txt", &a);
    printf("readme.txt is a regular file: %d, size %lld\n", S_ISREG(a.st_mode), (long long)a.st_size);

    // rename changes the name in the directory; the inode stays.
    rename("tree/readme.txt", "tree/README.txt");
    stat("tree/README.txt", &b);
    printf("same inode after rename: %d\n", a.st_ino == b.st_ino);

    // link adds a second name; st_nlink counts the names.
    link("tree/README.txt", "tree/link.txt");
    stat("tree/link.txt", &b);
    printf("same file: %d, links %ld\n", a.st_dev == b.st_dev && a.st_ino == b.st_ino, (long)b.st_nlink);

    // statvfs describes the file system that holds a path.
    struct statvfs v;
    if (statvfs("tree", &v) == 0) {
        printf("free bytes for this user > 0: %d\n", (unsigned long long)v.f_bavail * v.f_frsize > 0);
    }

    // A missing file gives -1 and errno ENOENT.
    if (stat("tree/missing.txt", &a) == -1) {
        printf("missing: %s\n", errno == ENOENT ? "ENOENT" : strerror(errno));
    }
    return 0;
}
```

```output for=posix-stat
tree is a directory: 1
readme.txt is a regular file: 1, size 8
same inode after rename: 1
same file: 1, links 2
free bytes for this user > 0: 1
missing: ENOENT
```

- Verdict: library. ISO C has `fopen`, `remove` and `rename`, but no directories and no file attributes. The program uses POSIX: `mkdir`, `stat` with `S_ISDIR` and `S_ISREG`, `link`, and `statvfs`. A rename keeps `st_ino`, a link raises `st_nlink` to 2, and a missing name gives `-1` with `errno` set to `ENOENT`.
