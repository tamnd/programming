```c run id=fnv-and-open-addressing
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* FNV-1a, 64 bits: fold each byte into the hash with xor, then multiply */
static uint64_t fnv1a(const void *data, size_t n, uint64_t h) {
    const unsigned char *p = data;
    for (size_t i = 0; i < n; i++)
        h = (h ^ p[i]) * 1099511628211u;
    return h;
}
#define FNV_START 14695981039346656037u

struct point { int32_t x, y; };

/* Hash the fields, not the bytes of the struct, so padding never counts */
static uint64_t point_hash(struct point p) {
    uint64_t h = fnv1a(&p.x, sizeof p.x, FNV_START);
    return fnv1a(&p.y, sizeof p.y, h);
}

/* A set of strings with open addressing and linear probing */
#define CAP 8
static const char *slots[CAP];

static int insert(const char *s) {
    size_t i = fnv1a(s, strlen(s), FNV_START) % CAP, probes = 1;
    while (slots[i] && strcmp(slots[i], s) != 0) {
        i = (i + 1) % CAP;
        probes++;
    }
    slots[i] = s;
    return (int)probes;
}

int main(void) {
    const char *words[] = {"ada", "alan", "grace", "linus", "ada", "barbara"};
    for (size_t i = 0; i < sizeof words / sizeof *words; i++)
        printf("%s: %d probe(s)\n", words[i], insert(words[i]));
    struct point a = {1, 2}, b = {1, 2}, c = {2, 1};
    printf("point hashes equal: %d, swapped fields equal: %d\n",
           point_hash(a) == point_hash(b), point_hash(a) == point_hash(c));
    printf("fnv1a(\"\") = %llu\n", (unsigned long long)fnv1a("", 0, FNV_START));
    return 0;
}
```

```output for=fnv-and-open-addressing
ada: 1 probe(s)
alan: 1 probe(s)
grace: 2 probe(s)
linus: 2 probe(s)
ada: 1 probe(s)
barbara: 1 probe(s)
point hashes equal: 1, swapped fields equal: 0
fnv1a("") = 14695981039346656037
```

- Verdict: idiom. The C standard library has no hash table and no hash function; POSIX `hsearch` has one global table. Programs write their own, and FNV-1a, shown here, is a common simple choice for short keys. The set uses open addressing: a key that finds its slot taken tries the next slot. The point hash mixes the fields one at a time, so the bytes of padding never enter the hash, and swapping the fields changes the result.
