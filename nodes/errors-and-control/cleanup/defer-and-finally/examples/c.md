```c run id=copy
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int open_count = 0;

static char *open_file(const char *name) {
    printf("open %s\n", name);
    open_count++;
    return strdup(name);
}

static void close_file(char *f) {
    printf("close %s\n", f);
    open_count--;
    free(f);
}

static int copy(const char *src, const char *dst, int fail) {
    int result = -1;
    char *a = open_file(src);
    if (a == NULL)
        goto out;
    char *b = open_file(dst);
    if (b == NULL)
        goto out_close_a;
    if (fail) {
        printf("cannot read %s\n", a);
        goto out_close_b;
    }
    printf("copy %s to %s\n", a, b);
    result = 0;
out_close_b:
    close_file(b);
out_close_a:
    close_file(a);
out:
    return result;
}

int main(void) {
    printf("result %d\n", copy("a.txt", "b.txt", 0));
    printf("result %d\n", copy("a.txt", "c.txt", 1));
    printf("files open: %d\n", open_count);
    return 0;
}
```

```output
open a.txt
open b.txt
copy a.txt to b.txt
close b.txt
close a.txt
result 0
open a.txt
open c.txt
cannot read a.txt
close c.txt
close a.txt
result -1
files open: 0
```

- Verdict: absent. C has no `finally`, no `defer` and no destructors. The cleanup code is written by hand.
- Each exit path jumps with `goto` to a label at the end of the function. The labels release the resources in reverse order of acquisition, and each label releases one resource.
- The Linux kernel coding style recommends this form and asks for label names that say what the label releases.

A `return` in the middle of the function skips the cleanup code. The program loses track of an open file.

```c wrong id=early-return
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int open_count = 0;

static char *open_file(const char *name) {
    open_count++;
    return strdup(name);
}

static void close_file(char *f) {
    open_count--;
    free(f);
}

static int copy(const char *src, int fail) {
    char *a = open_file(src);
    if (fail)
        return -1;
    printf("copy %s\n", a);
    close_file(a);
    return 0;
}

int main(void) {
    copy("a.txt", 0);
    copy("b.txt", 1);
    printf("files open: %d\n", open_count);
    return 0;
}
```

```output
copy a.txt
files open: 1
```
