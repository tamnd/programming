```c run id=either
#include <ctype.h>
#include <stdio.h>

enum kind { INT, TEXT };

struct int_or_text {
    enum kind kind; /* the tag: which member of the union holds the value */
    union {
        int i;
        const char *s;
    } as;
};

static void describe(struct int_or_text v) {
    switch (v.kind) {
    case INT:
        printf("number %d, doubled %d\n", v.as.i, v.as.i * 2);
        break;
    case TEXT:
        printf("text %s, upper ", v.as.s);
        for (const char *p = v.as.s; *p; p++) {
            putchar(toupper((unsigned char)*p));
        }
        putchar('\n');
        break;
    }
}

int main(void) {
    struct int_or_text values[] = {{.kind = INT, .as.i = 21}, {.kind = TEXT, .as.s = "ok"}};
    for (int i = 0; i < 2; i++) {
        describe(values[i]);
    }
    union {
        char c;
        double d;
    } u;
    printf("union size is the largest member: %s\n", sizeof u == sizeof(double) ? "yes" : "no");
    return 0;
}
```

```output for=either
number 21, doubled 42
text ok, upper OK
union size is the largest member: yes
```

- Verdict: native. A `union` stores one of its members at a time, all at the same address. It does not record which one.
- The struct adds a `kind` field as the tag. Each write sets the tag, and each read checks it first.
- The size of a union is the size of its largest member, plus any padding.
