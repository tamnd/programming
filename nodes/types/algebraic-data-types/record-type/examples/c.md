```c run id=record
#include <stdio.h>
#include <string.h>

struct person {
    const char *name;
    int age;
};

static void show(const char *label, struct person p) {
    printf("%s{name=%s, age=%d}\n", label, p.name, p.age);
}

static int person_equal(struct person a, struct person b) {
    return strcmp(a.name, b.name) == 0 && a.age == b.age;
}

int main(void) {
    struct person ada = {.name = "Ada", .age = 36};
    printf("name: %s age: %d\n", ada.name, ada.age);

    struct person older = ada; /* assignment copies every member */
    older.age += 1;
    show("copy: ", older);
    show("original: ", ada);

    struct person other = {.name = "Ada", .age = 36};
    printf("same fields equal: %s\n", person_equal(ada, other) ? "yes" : "no");
    return 0;
}
```

```output for=record
name: Ada age: 36
copy: {name=Ada, age=37}
original: {name=Ada, age=36}
same fields equal: yes
```

- Verdict: native. A `struct` is a record. Designated initializers such as `.age = 36` set fields by name.
- C has no `==` for structs, so `person_equal` compares the members. It compares the strings with `strcmp`, not the pointers.
- The copy is shallow: `older.name` points to the same characters as `ada.name`.
