```kotlin run id=vm
// The opcode numbers are the ordinals: PUSH = 0, LOAD = 1, ... HALT = 10.
enum class Op(val hasArg: Boolean) {
    PUSH(true), LOAD(true), STORE(true), ADD(false), SUB(false), MUL(false), LT(false),
    JUMP_IF_FALSE(true), JUMP(true), PRINT(false), HALT(false),
}

// let n = 5; let acc = 1; while (0 < n) { let step = n; acc = acc * step; n = n - 1; } print acc;
// Slots: n = 0, acc = 1, step = 2.
val code = byteArrayOf(
    0, 5, 2, 0, 0, 1, 2, 1,
    0, 0, 1, 0, 6, 7, 35,
    1, 0, 2, 2,
    1, 1, 1, 2, 5, 2, 1,
    1, 0, 0, 1, 4, 2, 0,
    8, 8,
    1, 1, 9, 10,
)

fun disassemble(code: ByteArray) {
    var pc = 0
    while (pc < code.size) {
        val op = Op.entries[code[pc].toInt()]
        if (op.hasArg) {
            println("%4d %-13s %d".format(pc, op, code[pc + 1]))
            pc += 2
        } else {
            println("%4d %s".format(pc, op))
            pc += 1
        }
    }
}

fun main() {
    disassemble(code)
    val stack = ArrayDeque<Int>()
    val slots = IntArray(3)
    var pc = 0
    var executed = 0
    var maxDepth = 0
    while (true) {
        val op = Op.entries[code[pc].toInt()]
        executed++
        val arg = if (op.hasArg) code[pc + 1].toInt() else 0
        pc += if (op.hasArg) 2 else 1
        when (op) {
            Op.PUSH -> stack.addLast(arg)
            Op.LOAD -> stack.addLast(slots[arg])
            Op.STORE -> slots[arg] = stack.removeLast()
            Op.ADD, Op.SUB, Op.MUL, Op.LT -> {
                val b = stack.removeLast() // the right operand is on top
                val a = stack.removeLast()
                stack.addLast(
                    when (op) {
                        Op.ADD -> a + b
                        Op.SUB -> a - b
                        Op.MUL -> a * b
                        else -> if (a < b) 1 else 0
                    }
                )
            }
            Op.JUMP_IF_FALSE -> if (stack.removeLast() == 0) pc = arg
            Op.JUMP -> pc = arg
            Op.PRINT -> println(stack.removeLast())
            Op.HALT -> break
        }
        maxDepth = maxOf(maxDepth, stack.size)
    }
    println("bytes of code: ${code.size}")
    println("instructions executed: $executed, maximum stack depth: $maxDepth")
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

The second program reads its own class file. The Kotlin compiler for the JVM writes the same format as `javac`, so the file starts with the same magic number.

```kotlin run id=magic
class Probe {
    fun twice(x: Int) = x * 2
}

fun main() {
    val bytes = Probe::class.java.getResourceAsStream("Probe.class")!!.use { it.readBytes() }
    println("magic: " + bytes.take(4).joinToString("") { "%02X".format(it) })
    println("twice(21) = ${Probe().twice(21)}")
}
```

```output for=magic
magic: CAFEBABE
twice(21) = 42
```

- Verdict: native. On the JVM target, Kotlin compiles to class files that hold JVM bytecode, the same format that Java uses.
- `Op.entries` turns an opcode number back into an enum constant. `when` over an enum needs no `else` branch when every constant has a case.
- `ArrayDeque` from the Kotlin standard library serves as the operand stack, with `addLast` as push and `removeLast` as pop.
