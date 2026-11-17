```c run id=three-kinds
#include <stdio.h>
#include <string.h>

struct count {
    const char *word;
    int n;
};

/* Find word in the table, or add it. Return its entry. */
static struct count *lookup(struct count *table, size_t *len, const char *word) {
    for (size_t i = 0; i < *len; i++) {
        if (strcmp(table[i].word, word) == 0) {
            return &table[i];
        }
    }
    table[*len].word = word;
    table[*len].n = 0;
    return &table[(*len)++];
}

int main(void) {
    const char *words[] = {"red", "blue", "red", "green"};
    size_t nwords = sizeof words / sizeof words[0];

    struct count table[4];
    size_t len = 0;
    for (size_t i = 0; i < nwords; i++) {
        lookup(table, &len, words[i])->n++;
    }

    printf("%zu %s\n", nwords, words[1]);
    printf("%zu", len);
    for (size_t i = 0; i < len; i++) {
        printf(" %s=%d", table[i].word, table[i].n);
    }
    printf("\n");
    return 0;
}
```

```output for=three-kinds
4 blue
3 red=2 blue=1 green=1
```

- Verdict: idiom. C has fixed arrays and structs. The program writes its own collections, or uses a library.
- The table is an array of structs that the code searches from the start, so each lookup takes time in proportion to the number of words. A real program would use a hash table.
- The size of the array is fixed when the program is compiled. A collection that grows needs `malloc` and `realloc`.
