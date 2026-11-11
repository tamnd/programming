```c run id=decompose
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct count {
    char word[16];
    int n;
};

static int split_words(char *text, char *words[], int max)
{
    int n = 0;
    for (char *p = text; *p; p++)
        *p = (char)tolower((unsigned char)*p);
    for (char *w = strtok(text, " "); w && n < max; w = strtok(NULL, " "))
        words[n++] = w;
    return n;
}

static int count_words(char *words[], int nwords, struct count out[])
{
    int n = 0;
    for (int i = 0; i < nwords; i++) {
        int j = 0;
        while (j < n && strcmp(out[j].word, words[i]) != 0)
            j++;
        if (j == n) {
            snprintf(out[n].word, sizeof out[n].word, "%s", words[i]);
            out[n++].n = 0;
        }
        out[j].n++;
    }
    return n;
}

static int by_count(const void *a, const void *b)
{
    const struct count *x = a, *y = b;
    if (x->n != y->n)
        return y->n - x->n;
    return strcmp(x->word, y->word);
}

static void report(const char *text, int top)
{
    char buf[256];
    char *words[64];
    struct count counts[64];

    snprintf(buf, sizeof buf, "%s", text);
    int nwords = split_words(buf, words, 64);
    int n = count_words(words, nwords, counts);
    qsort(counts, (size_t)n, sizeof counts[0], by_count);
    for (int i = 0; i < top && i < n; i++)
        printf("%-5s %d\n", counts[i].word, counts[i].n);
}

int main(void)
{
    report("The cat and the hat and the bat", 2);
    return 0;
}
```

```output for=decompose
the   3
and   2
```

- Verdict: native. C is the standard procedural language. A program is a set of functions, and cppreference describes a function as a construct "that associates a compound statement (the function body) with an identifier (the function name)".
- `static` at file level makes a function private to its source file. A header file declares the functions that other files may call.
