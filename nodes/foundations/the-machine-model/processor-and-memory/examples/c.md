```c run id=machine-in-long-array
#include <stddef.h>
#include <stdio.h>

enum { HALT, LOAD, ADD, SUB, STORE, JZ, JMP };

static int run(long mem[]) {
    long acc = 0;
    int pc = 0, steps = 0;
    for (;;) {
        long op = mem[pc], addr = mem[pc + 1];
        pc += 2;
        steps++;
        switch (op) {
        case HALT: return steps;
        case LOAD: acc = mem[addr]; break;
        case ADD: acc += mem[addr]; break;
        case SUB: acc -= mem[addr]; break;
        case STORE: mem[addr] = acc; break;
        case JZ: if (acc == 0) pc = (int)addr; break;
        case JMP: pc = (int)addr; break;
        }
    }
}

struct cell {
    char tag;
    long value;
};

int main(void) {
    long mem[] = {LOAD, 20, JZ, 18, LOAD, 21, ADD, 20, STORE, 21,
                  LOAD, 20, SUB, 22, STORE, 20, JMP, 0, HALT, 0, 5, 0, 1};
    int steps = run(mem);
    printf("sum=%ld steps=%d\n", mem[21], steps);
    printf("%zu %zu %zu\n", sizeof(void *), sizeof(struct cell), offsetof(struct cell, value));
    printf("%td\n", (char *)&mem[1] - (char *)&mem[0]);
    return 0;
}
```

```output for=machine-in-long-array
sum=15 steps=48
8 16 8
8
```

- Verdict: native. C is close to the machine model: memory is an array of bytes with addresses, a pointer holds an address, and `sizeof` and `offsetof` show how the compiler lays out a struct. In the example, the `char` field takes 1 byte and the compiler adds 7 bytes of padding so that the `long` starts at an address that is a multiple of 8. Two neighbouring `long` elements are 8 bytes apart.
- C does not check indices, so a read past the end of an array reads other memory, which is undefined behavior.
