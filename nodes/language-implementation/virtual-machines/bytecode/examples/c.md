```c run id=vm
#include <stdio.h>
#include <stdlib.h>

enum op { PUSH, LOAD, STORE, ADD, SUB, MUL, LT, JUMP_IF_FALSE, JUMP, PRINT, HALT };

static const char *const names[] = {
    "PUSH", "LOAD", "STORE", "ADD", "SUB", "MUL", "LT", "JUMP_IF_FALSE", "JUMP", "PRINT", "HALT",
};

static int has_arg(unsigned char op) {
    return op == PUSH || op == LOAD || op == STORE || op == JUMP_IF_FALSE || op == JUMP;
}

/* let n = 5; let acc = 1; while (0 < n) { let step = n; acc = acc * step; n = n - 1; } print acc;
   Slots: n = 0, acc = 1, step = 2. */
static const unsigned char code[] = {
    PUSH, 5, STORE, 0, PUSH, 1, STORE, 1,
    PUSH, 0, LOAD, 0, LT, JUMP_IF_FALSE, 35,
    LOAD, 0, STORE, 2,
    LOAD, 1, LOAD, 2, MUL, STORE, 1,
    LOAD, 0, PUSH, 1, SUB, STORE, 0,
    JUMP, 8,
    LOAD, 1, PRINT, HALT,
};

static void disassemble(const unsigned char *code, size_t len) {
    for (size_t pc = 0; pc < len;) {
        if (has_arg(code[pc])) {
            printf("%4zu %-13s %d\n", pc, names[code[pc]], code[pc + 1]);
            pc += 2;
        } else {
            printf("%4zu %s\n", pc, names[code[pc]]);
            pc += 1;
        }
    }
}

enum { STACK_MAX = 16 };

static void run(const unsigned char *code, int *executed, int *max_depth) {
    long stack[STACK_MAX];
    long slots[3] = {0}; /* n, acc, step */
    int sp = 0; /* the number of values on the stack */
    size_t pc = 0;
    *executed = 0;
    *max_depth = 0;
    for (;;) {
        unsigned char op = code[pc];
        int arg = 0;
        ++*executed;
        if (has_arg(op)) {
            arg = code[pc + 1];
            pc += 2;
        } else {
            pc += 1;
        }
        switch (op) {
        case PUSH: stack[sp++] = arg; break;
        case LOAD: stack[sp++] = slots[arg]; break;
        case STORE: slots[arg] = stack[--sp]; break;
        case ADD: sp--; stack[sp - 1] = stack[sp - 1] + stack[sp]; break;
        case SUB: sp--; stack[sp - 1] = stack[sp - 1] - stack[sp]; break;
        case MUL: sp--; stack[sp - 1] = stack[sp - 1] * stack[sp]; break;
        case LT: sp--; stack[sp - 1] = stack[sp - 1] < stack[sp]; break;
        case JUMP_IF_FALSE: if (stack[--sp] == 0) pc = (size_t)arg; break;
        case JUMP: pc = (size_t)arg; break;
        case PRINT: printf("%ld\n", stack[--sp]); break;
        case HALT: return;
        default: fprintf(stderr, "bad opcode %d\n", op); exit(1);
        }
        if (sp > *max_depth) *max_depth = sp;
    }
}

int main(void) {
    int executed, max_depth;
    disassemble(code, sizeof code);
    run(code, &executed, &max_depth);
    printf("bytes of code: %zu\n", sizeof code);
    printf("instructions executed: %d, maximum stack depth: %d\n", executed, max_depth);
    return 0;
}
```

```output for=vm
   0 PUSH          5
   2 STORE         0
   4 PUSH          1
   6 STORE         1
   8 PUSH          0
  10 LOAD          0
  12 LT
  13 JUMP_IF_FALSE 35
  15 LOAD          0
  17 STORE         2
  19 LOAD          1
  21 LOAD          2
  23 MUL
  24 STORE         1
  26 LOAD          0
  28 PUSH          1
  30 SUB
  31 STORE         0
  33 JUMP          8
  35 LOAD          1
  37 PRINT
  38 HALT
120
bytes of code: 39
instructions executed: 86, maximum stack depth: 2
```

- Verdict: idiom. C compiles ahead of time to machine code, so the program builds its own bytecode.
- The stack is a fixed array with an index `sp`. A binary operator pops one value and overwrites the new top in place, so it moves `sp` once instead of three times.
- Nothing checks `sp`. Code that pops an empty stack reads before the array, which is undefined behaviour. A real VM verifies the code first, as the JVM does, or checks at run time.
- The enum constants appear directly in the code array, so `sizeof code` gives the byte count without a separate length.
