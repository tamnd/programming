```c run id=extern-and-tentative-definitions
#include <stdio.h>

extern int total;          /* a declaration: the object is defined below */
int sum(int a, int b);     /* a function declaration (a prototype) */
struct node;               /* an incomplete type: a declaration only */

int tentative;             /* a tentative definition */
int tentative;             /* another one is valid, and they name one object */

struct list { struct node *head; int size; };

int main(void) {
    printf("sum(2, 3) = %d\n", sum(2, 3));
    total += 5;
    printf("total = %d\n", total);
    printf("tentative = %d\n", tentative);
    struct list l = { NULL, 0 };
    printf("l.size = %d\n", l.size);
    return 0;
}

int total = 10;                           /* the definition of total */
int sum(int a, int b) { return a + b; }   /* the definition of sum */
```

```output for=extern-and-tentative-definitions
sum(2, 3) = 5
total = 15
tentative = 0
l.size = 0
```

- Verdict: native. `extern int total;` and the prototype of `sum` are declarations, and the definitions come after `main`. `struct node;` declares an incomplete type, and a pointer to it is valid. Two declarations `int tentative;` are tentative definitions of one object, which has the value `0`.
