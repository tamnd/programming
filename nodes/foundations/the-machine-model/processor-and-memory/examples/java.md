```java run id=machine-in-int-array
public class Main {
    static final int HALT = 0, LOAD = 1, ADD = 2, SUB = 3, STORE = 4, JZ = 5, JMP = 6;

    static int run(int[] mem) {
        int pc = 0, acc = 0, steps = 0;
        while (true) {
            int op = mem[pc], addr = mem[pc + 1];
            pc += 2;
            steps++;
            switch (op) {
                case HALT -> { return steps; }
                case LOAD -> acc = mem[addr];
                case ADD -> acc += mem[addr];
                case SUB -> acc -= mem[addr];
                case STORE -> mem[addr] = acc;
                case JZ -> { if (acc == 0) pc = addr; }
                case JMP -> pc = addr;
                default -> throw new IllegalStateException("bad opcode " + op);
            }
        }
    }

    public static void main(String[] args) {
        int[] mem = {LOAD, 20, JZ, 18, LOAD, 21, ADD, 20, STORE, 21,
                     LOAD, 20, SUB, 22, STORE, 20, JMP, 0, HALT, 0, 5, 0, 1};
        int steps = run(mem);
        System.out.println("sum=" + mem[21] + " steps=" + steps);
        System.out.println(Integer.SIZE + " " + Long.SIZE);
    }
}
```

```output for=machine-in-int-array
sum=15 steps=48
32 64
```

- Verdict: stdlib. Java runs on the Java Virtual Machine, an abstract machine with its own program counter, operand stack and heap, and the JIT compiler maps it to the real processor. The language fixes the widths of its types on every machine, 32 bits for `int` and 64 for `long`, and has no pointers that a program can see. The Foreign Function and Memory API of the standard library reads and writes memory outside the heap through `MemorySegment`.
