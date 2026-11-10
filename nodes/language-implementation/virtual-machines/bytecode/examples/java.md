```java run id=vm
import java.util.ArrayDeque;
import java.util.Deque;

public class Main {
    // The opcode numbers are the ordinals: PUSH = 0, LOAD = 1, ... HALT = 10.
    enum Op {
        PUSH(true), LOAD(true), STORE(true), ADD(false), SUB(false), MUL(false), LT(false),
        JUMP_IF_FALSE(true), JUMP(true), PRINT(false), HALT(false);

        final boolean hasArg;
        Op(boolean hasArg) { this.hasArg = hasArg; }
        static final Op[] ALL = values();
    }

    // let n = 5; let acc = 1; while (0 < n) { let step = n; acc = acc * step; n = n - 1; } print acc;
    // Slots: n = 0, acc = 1, step = 2.
    static final byte[] CODE = {
        0, 5, 2, 0, 0, 1, 2, 1,
        0, 0, 1, 0, 6, 7, 35,
        1, 0, 2, 2,
        1, 1, 1, 2, 5, 2, 1,
        1, 0, 0, 1, 4, 2, 0,
        8, 8,
        1, 1, 9, 10,
    };

    static void disassemble(byte[] code) {
        for (int pc = 0; pc < code.length; ) {
            Op op = Op.ALL[code[pc]];
            if (op.hasArg) {
                System.out.println(String.format("%4d %-13s %d", pc, op, code[pc + 1]));
                pc += 2;
            } else {
                System.out.println(String.format("%4d %s", pc, op));
                pc += 1;
            }
        }
    }

    public static void main(String[] args) {
        disassemble(CODE);
        Deque<Integer> stack = new ArrayDeque<>();
        int[] slots = new int[3];
        int pc = 0, executed = 0, maxDepth = 0;
        loop:
        for (;;) {
            Op op = Op.ALL[CODE[pc]];
            executed++;
            int arg = op.hasArg ? CODE[pc + 1] : 0;
            pc += op.hasArg ? 2 : 1;
            switch (op) {
                case PUSH -> stack.push(arg);
                case LOAD -> stack.push(slots[arg]);
                case STORE -> slots[arg] = stack.pop();
                case ADD, SUB, MUL, LT -> {
                    int b = stack.pop(); // the right operand is on top
                    int a = stack.pop();
                    stack.push(switch (op) {
                        case ADD -> a + b;
                        case SUB -> a - b;
                        case MUL -> a * b;
                        default -> a < b ? 1 : 0;
                    });
                }
                case JUMP_IF_FALSE -> { if (stack.pop() == 0) pc = arg; }
                case JUMP -> pc = arg;
                case PRINT -> System.out.println(stack.pop());
                case HALT -> { break loop; }
            }
            maxDepth = Math.max(maxDepth, stack.size());
        }
        System.out.println("bytes of code: " + CODE.length);
        System.out.println("instructions executed: " + executed + ", maximum stack depth: " + maxDepth);
    }
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

The second program compiles a small class at run time with the compiler in `javax.tools`, then runs `javap -c` on the class file through `java.util.spi.ToolProvider`. Both tools ship with the JDK.

```java run id=javap
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Comparator;
import java.util.spi.ToolProvider;
import java.util.stream.Stream;
import javax.tools.JavaCompiler;

public class Main {
    static final String SOURCE = """
        class Fact {
            static int fact(int n) {
                int acc = 1;
                while (0 < n) {
                    acc = acc * n;
                    n = n - 1;
                }
                return acc;
            }
        }
        """;

    public static void main(String[] args) throws IOException {
        Path dir = Files.createTempDirectory("bytecode");
        try {
            Path src = dir.resolve("Fact.java");
            Files.writeString(src, SOURCE);
            JavaCompiler javac = javax.tools.ToolProvider.getSystemJavaCompiler();
            int status = javac.run(null, null, null, "-d", dir.toString(), src.toString());
            if (status != 0) throw new IllegalStateException("javac failed");

            byte[] bytes = Files.readAllBytes(dir.resolve("Fact.class"));
            System.out.printf("magic: %02X%02X%02X%02X%n", bytes[0], bytes[1], bytes[2], bytes[3]);

            ToolProvider javap = ToolProvider.findFirst("javap").orElseThrow();
            javap.run(System.out, System.err, "-c", dir.resolve("Fact.class").toString());
        } finally {
            try (Stream<Path> files = Files.walk(dir)) {
                files.sorted(Comparator.reverseOrder()).forEach(p -> p.toFile().delete());
            }
        }
    }
}
```

```output for=javap
magic: CAFEBABE
Compiled from "Fact.java"
class Fact {
  Fact();
    Code:
         0: aload_0
         1: invokespecial #1                  // Method java/lang/Object."<init>":()V
         4: return

  static int fact(int);
    Code:
         0: iconst_1
         1: istore_1
         2: iconst_0
         3: iload_0
         4: if_icmpge     18
         7: iload_1
         8: iload_0
         9: imul
        10: istore_1
        11: iload_0
        12: iconst_1
        13: isub
        14: istore_0
        15: goto          2
        18: iload_1
        19: ireturn
}
```

- Verdict: native. `javac` translates Java to JVM bytecode in class files, and the JVM runs that bytecode.
- The JVM is a stack machine, as the `javap` output shows. `iload_1` pushes local variable 1, `imul` pops two `int` values and pushes the product, and `istore_1` pops into local 1. The method is static, so local 0 holds the argument `n`.
- The JVM opcodes carry the type in their name: `iload` and `imul` work on `int`, `dload` and `dmul` on `double`. The toy machine has one type, so it needs only one `LOAD` and one `MUL`.
- The VM uses an `enum` and an exhaustive `switch`. A `byte` in Java is signed, so an operand above 127 would need `code[pc + 1] & 0xFF`.
