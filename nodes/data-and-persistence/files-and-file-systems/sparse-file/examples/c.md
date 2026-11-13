```c run id=seek-data-and-hole
#define _GNU_SOURCE /* SEEK_DATA and SEEK_HOLE on glibc */
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main(void) {
    char path[] = "/tmp/sparse-XXXXXX";
    int fd = mkstemp(path);
    if (fd < 0) {
        perror("mkstemp");
        return 1;
    }
    const off_t mib = 1024 * 1024;
    /* Data at 64 MiB, then a size of 128 MiB: a hole, data, and a hole at the end. */
    if (pwrite(fd, "data", 4, 64 * mib) != 4 || ftruncate(fd, 128 * mib) != 0) {
        perror("write");
        return 1;
    }

    struct stat st;
    fstat(fd, &st);
    printf("size:            %lld\n", (long long)st.st_size);
    printf("uses less space: %s\n", (long long)st.st_blocks * 512 < (long long)st.st_size ? "yes" : "no");

    off_t data = lseek(fd, 0, SEEK_DATA);
    off_t hole = lseek(fd, data, SEEK_HOLE);
    printf("data at:         %lld\n", (long long)data);
    printf("data ends before %s 65 MiB\n", hole <= 65 * mib ? "or at" : "after");

    /* The offset is in the hole at the end, so no data follows it. */
    errno = 0;
    off_t none = lseek(fd, 96 * mib, SEEK_DATA);
    printf("after 96 MiB:    %lld, %s\n", (long long)none, errno == ENXIO ? "ENXIO" : strerror(errno));

    close(fd);
    unlink(path);
    return 0;
}
```

```output for=seek-data-and-hole
size:            134217728
uses less space: yes
data at:         67108864
data ends before or at 65 MiB
after 96 MiB:    -1, ENXIO
```

- Verdict: library. ISO C has no holes. POSIX `lseek` past the end, `pwrite` and `ftruncate` make them, and `SEEK_DATA` and `SEEK_HOLE` find them. A file system can round a run of data to its block size, so the example checks only that the data ends before 65 MiB. On glibc, the two constants need `_GNU_SOURCE`.
