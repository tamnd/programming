```c run id=count
#include <stdbool.h>
#include <stdio.h>

enum light { RED, AMBER, GREEN };
static const char *light_name[] = { "red", "amber", "green" };

/* Product: a flag AND a light. */
struct pair {
    bool flag;
    enum light light;
};

/* Sum: a flag OR a light. The tag records which member of the union holds a value. */
enum choice_tag { FLAG, LAMP };
struct choice {
    enum choice_tag tag;
    union {
        bool flag;
        enum light light;
    } as;
};

static const char *yn(bool b) { return b ? "yes" : "no"; }

int main(void) {
    struct pair pairs[2 * 3];
    struct choice choices[2 + 3];
    int np = 0, nc = 0;
    for (int f = 0; f < 2; f++) {
        for (enum light l = RED; l <= GREEN; l++) {
            pairs[np++] = (struct pair){ .flag = f, .light = l };
        }
        choices[nc++] = (struct choice){ .tag = FLAG, .as.flag = f };
    }
    for (enum light l = RED; l <= GREEN; l++) {
        choices[nc++] = (struct choice){ .tag = LAMP, .as.light = l };
    }

    printf("pairs: %d\n", np);
    for (int i = 0; i < np; i++) {
        printf("%s%s/%s", i ? " " : "", yn(pairs[i].flag), light_name[pairs[i].light]);
    }
    printf("\nchoices: %d\n", nc);
    for (int i = 0; i < nc; i++) {
        printf("%s", i ? " " : "");
        switch (choices[i].tag) {
        case FLAG:
            printf("flag:%s", yn(choices[i].as.flag));
            break;
        case LAMP:
            printf("light:%s", light_name[choices[i].as.light]);
            break;
        }
    }
    printf("\n");
    return 0;
}
```

```output for=count
pairs: 6
no/red no/amber no/green yes/red yes/amber yes/green
choices: 5
flag:no flag:yes light:red light:amber light:green
```

- Verdict: idiom. A `struct` is a product. A `struct` with an `enum` tag and a `union` is a sum.
- Nothing ties the tag to the member of the union. The program must set and test the tag itself.
- The arrays have the sizes 2 × 3 and 2 + 3, the counts of the product and the sum.
