```c run id=fallthrough-goto-and-longjmp
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

static jmp_buf env;

static void inner(int fail) {
    if (fail) {
        longjmp(env, 42); /* jump back to setjmp, through all callers */
    }
    printf("inner ok\n");
}

/* goto to one cleanup label per resource is a common C idiom. */
static int work(int fail) {
    int rc = -1;
    char *a = malloc(16);
    if (a == NULL) goto out;
    char *b = fail ? NULL : malloc(16); /* fail simulates an allocation failure */
    if (b == NULL) goto free_a;
    printf("both allocated ");
    rc = 0;
    free(b);
free_a:
    free(a);
out:
    return rc;
}

int main(void) {
    /* A switch case falls through into the next case unless it breaks. */
    for (int n = 1; n <= 3; n++) {
        switch (n) {
        case 1:
            printf("one ");
            /* fall through */
        case 2:
            printf("one-or-two ");
            break;
        default:
            printf("other ");
        }
    }
    printf("\n");

    int ok = work(0);
    int failed = work(1);
    printf("%d %d\n", ok, failed);

    int code = setjmp(env);
    if (code == 0) {
        inner(0);
        inner(1);
        printf("not reached\n");
    } else {
        printf("longjmp returned %d\n", code);
    }
    return 0;
}
```

```output for=fallthrough-goto-and-longjmp
one one-or-two one-or-two other 
both allocated 0 -1
inner ok
longjmp returned 42
```

- Verdict: native. A `switch` case without `break` falls through, so `1` prints both labels. The `goto` cleanup idiom jumps to the label that frees what was allocated, and the simulated failure returns `-1`. `longjmp` jumps from `inner` back to `setjmp` in `main`, so the line after the second call does not run.
