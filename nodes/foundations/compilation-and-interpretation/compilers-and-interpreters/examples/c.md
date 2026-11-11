```c run id=bytecode-interpreter
#include <stdio.h>

enum op { PUSH, LOAD, ADD, MUL, HALT };

struct ins {
    enum op op;
    int arg;
};

/* A bytecode interpreter: fetch an instruction, decode it, execute it. */
static int run(const struct ins *code, int x) {
    int stack[16], sp = 0;
    for (const struct ins *pc = code;; pc++) {
        switch (pc->op) {
        case PUSH: stack[sp++] = pc->arg; break;
        case LOAD: stack[sp++] = x; break;
        case ADD: sp--; stack[sp - 1] += stack[sp]; break;
        case MUL: sp--; stack[sp - 1] *= stack[sp]; break;
        case HALT: return stack[sp - 1];
        }
    }
}

/* The same formula compiled by the C compiler to machine code. */
static int native(int x) { return (x * 2 + 1) * x; }

int main(void) {
    const struct ins code[] = {{LOAD, 0}, {PUSH, 2}, {MUL, 0}, {PUSH, 1},
                               {ADD, 0},  {LOAD, 0}, {MUL, 0}, {HALT, 0}};
    for (int x = 1; x <= 10; x += 4) printf("%d %d\n", run(code, x), native(x));
    _Static_assert(sizeof code / sizeof code[0] == 8, "eight instructions"); /* checked at compile time */
    return 0;
}
```

```output for=bytecode-interpreter
3 3
55 55
171 171
```

- Verdict: native. A C compiler translates each source file to machine code in an object file, and a linker joins the object files and libraries into an executable. The preprocessor runs first, and `_Static_assert` lets the compiler check a condition before the program exists.
- Many interpreters, including CPython and the Lua and Ruby virtual machines, are written in C around a loop like `run`: fetch an instruction, decode it with a `switch`, and execute it. The same formula compiled to machine code by the C compiler gives the same results without the dispatch work.
