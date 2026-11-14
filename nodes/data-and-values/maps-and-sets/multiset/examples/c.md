```c run id=count-array
#include <ctype.h>
#include <stdio.h>
#include <string.h>

/* For a small, known alphabet, a multiset is an array of counts. */
static void count(const char *s, int c[26]) {
    memset(c, 0, 26 * sizeof c[0]);
    for (; *s; s++)
        if (isalpha((unsigned char)*s)) c[tolower((unsigned char)*s) - 'a']++;
}

/* Is every count of a less than or equal to the count in b? */
static int subset(const int a[26], const int b[26]) {
    for (int i = 0; i < 26; i++)
        if (a[i] > b[i]) return 0;
    return 1;
}

int main(void) {
    int a[26], b[26];
    count("Listen", a);
    count("Silent", b);
    printf("anagram %d\n", memcmp(a, b, sizeof a) == 0);

    count("aab", a);
    count("ab", b);
    printf("anagram %d\n", memcmp(a, b, sizeof a) == 0);

    /* Can the letters of a note be cut from a magazine page? */
    count("cab", a);
    count("abracadabra", b);
    printf("subset %d %d\n", subset(a, b), subset(b, a));

    /* The size of the multiset and the number of distinct letters. */
    int total = 0, distinct = 0;
    for (int i = 0; i < 26; i++) {
        total += b[i];
        distinct += b[i] > 0;
    }
    printf("%d %d a=%d\n", total, distinct, b[0]);
    return 0;
}
```

```output
anagram 1
anagram 0
subset 1 0
11 5 a=5
```

- Verdict: idiom. For a small known alphabet, a multiset is an array of counts. memcmp compares two of them, and a loop over the counts tests whether one multiset is inside another.
