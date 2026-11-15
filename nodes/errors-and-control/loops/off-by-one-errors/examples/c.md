```c run id=buffer-room-for-the-null-and-fenceposts
#include <stdio.h>
#include <string.h>

int main(void) {
    /* Fenceposts: 30 feet with a post every 3 feet. */
    int sections = 30 / 3;
    printf("posts: %d\n", sections + 1);

    /* "hello" needs 6 bytes: 5 letters and the null character. */
    const char *word = "hello";
    char buf[6];
    printf("strlen %zu, needs %zu\n", strlen(word), strlen(word) + 1);
    if (strlen(word) < sizeof buf) {
        memcpy(buf, word, strlen(word) + 1);
        printf("copied: %s\n", buf);
    }

    /* snprintf writes at most size - 1 characters and a null. */
    char small[4];
    int n = snprintf(small, sizeof small, "%s", word);
    printf("wanted %d, got \"%s\"\n", n, small);

    /* The half-open loop over an array: i < n, never i <= n. */
    int a[] = {3, 1, 4, 1, 5};
    size_t len = sizeof a / sizeof a[0];
    int sum = 0;
    for (size_t i = 0; i < len; i++)
        sum += a[i];
    printf("sum %d of %zu items, last a[%zu] = %d\n", sum, len, len - 1, a[len - 1]);
    return 0;
}
```

```output for=buffer-room-for-the-null-and-fenceposts
posts: 11
strlen 5, needs 6
copied: hello
wanted 5, got "hel"
sum 14 of 5 items, last a[4] = 5
```

- Verdict: idiom. C checks no index, so the loop `i <= len` would read `a[5]`, past the end, with undefined behavior. The code uses `i < len`. A string of 5 letters needs a buffer of 6 bytes for the null character, which is the classic fencepost of C strings. `snprintf` writes at most `size - 1` characters and the null, and it returns the length it wanted, so a caller can see the cut.
