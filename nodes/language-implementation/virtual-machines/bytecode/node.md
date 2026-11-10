---
tag: NEW-bytecode
kind: concept
slug: bytecode
name: Bytecode
aliases: [byte code, p-code, portable code, intermediate code, virtual machine code]
area: language-implementation/virtual-machines
level: core
summary: >-
  Bytecode is a compact list of instructions for a virtual machine, which a program interprets one instruction at a time. A compiler produces it from the syntax tree, once, before the program runs.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 2005
  people: [Yunhe Shi, David Gregg, Andrew Beatty, M. Anton Ertl]
  note: Shi, Gregg, Beatty and Ertl measured stack and register forms of the same virtual machine code and found that the register form executed fewer instructions with larger code.
see_also: [tree-walking-interpreter, abstract-syntax-tree, just-in-time-compilation, recursive-descent-parsing]
same_as:
  wikidata: Q837330
  wikipedia: Bytecode
sources:
  - url: https://craftinginterpreters.com/chunks-of-bytecode.html#what-is-bytecode
    title: Robert Nystrom, Crafting Interpreters, chapter 14, Chunks of Bytecode
    locator: 14.1.3 What is bytecode?
  - url: https://craftinginterpreters.com/chunks-of-bytecode.html#why-not-walk-the-ast
    title: Robert Nystrom, Crafting Interpreters, chapter 14, Chunks of Bytecode
    locator: 14.1.1 Why not walk the AST?
  - url: https://docs.oracle.com/javase/specs/jvms/se25/html/jvms-1.html#jvms-1.2
    title: The Java Virtual Machine Specification, Java SE 25 Edition, chapter 1, Introduction
    locator: 1.2 The Java Virtual Machine
  - url: https://docs.oracle.com/javase/specs/jvms/se25/html/jvms-2.html#jvms-2.6.2
    title: The Java Virtual Machine Specification, Java SE 25 Edition, chapter 2, The Structure of the Java Virtual Machine
    locator: 2.6.2 Operand Stacks
  - url: https://docs.oracle.com/javase/specs/jvms/se25/html/jvms-2.html#jvms-2.11
    title: The Java Virtual Machine Specification, Java SE 25 Edition, chapter 2, The Structure of the Java Virtual Machine
    locator: 2.11 Instruction Set Summary
  - url: https://docs.oracle.com/javase/specs/jvms/se25/html/jvms-4.html#jvms-4.7.3
    title: The Java Virtual Machine Specification, Java SE 25 Edition, chapter 4, The class File Format
    locator: 4.7.3 The Code Attribute (max_stack, max_locals)
  - url: https://docs.oracle.com/javase/specs/jvms/se25/html/jvms-4.html#jvms-4.10
    title: The Java Virtual Machine Specification, Java SE 25 Edition, chapter 4, The class File Format
    locator: 4.10 Verification of class Files
  - url: https://docs.oracle.com/en/java/javase/25/docs/specs/man/javap.html
    title: The javap Command, Java SE 25 tool specifications
    locator: Synopsis and Options (-c)
  - url: https://docs.python.org/3.14/library/dis.html#module-dis
    title: The Python Standard Library, dis, Disassembler for Python bytecode, Python 3.14
    locator: module introduction (CPython implementation detail; changes in 3.6 and 3.11)
  - url: https://learn.microsoft.com/en-us/dotnet/framework/tools/ildasm-exe-il-disassembler
    title: Microsoft Learn, Ildasm.exe (IL Disassembler)
    locator: introduction
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.reflection.emit.opcodes.add
    title: Microsoft Learn, OpCodes.Add Field
    locator: Remarks (stack transitional behavior)
  - url: https://learn.microsoft.com/en-us/dotnet/standard/managed-execution-process#compile-to-cil
    title: Microsoft Learn, Managed execution process
    locator: Compile to CIL
  - url: https://craftinginterpreters.com/a-virtual-machine.html#binary-operators
    title: Robert Nystrom, Crafting Interpreters, chapter 15, A Virtual Machine
    locator: 15.3.1 Binary operators (the right operand is on top of the stack)
  - url: https://craftinginterpreters.com/a-virtual-machine.html#design-note
    title: Robert Nystrom, Crafting Interpreters, chapter 15, A Virtual Machine
    locator: Design Note, Register-Based Bytecode
  - paper: doi:10.1145/1064979.1065001
    title: Yunhe Shi, David Gregg, Andrew Beatty and M. Anton Ertl, Virtual Machine Showdown, Stack Versus Registers, Proceedings of the 1st ACM/USENIX International Conference on Virtual Execution Environments (VEE '05), 2005, pages 153 to 163
    locator: Abstract; section 1, Introduction
  - url: https://v8.dev/blog/ignition-interpreter
    title: Ross McIlroy, Firing up the Ignition interpreter, V8 blog, 23 August 2016
    locator: paragraphs on bytecode size and on the register machine with an accumulator
  - url: https://learn.microsoft.com/en-us/dotnet/api/system.reflection.methodbody.getilasbytearray
    title: Microsoft Learn, MethodBody.GetILAsByteArray Method
    locator: Returns
  - url: https://docs.oracle.com/javase/specs/jvms/se25/html/jvms-4.html#jvms-4.1
    title: The Java Virtual Machine Specification, Java SE 25 Edition, chapter 4, The class File Format
    locator: 4.1 The ClassFile Structure (magic)
---

## Idea

A [[tree-walking-interpreter]] runs a program from its syntax tree. That is slow, because the nodes are separate objects scattered over the heap, and each step follows pointers and dispatches on the type of a node.[^2] Bytecode moves the work to an earlier point. A compiler walks the tree once and writes a flat, dense list of instructions. A virtual machine then runs the list in a loop.

The instruction set belongs to a machine that does not exist in hardware. It is an idealized instruction set that makes the work of the compiler writer easier, and a program written in a portable language, the virtual machine, emulates it one instruction at a time.[^1] In many formats each opcode fits in one byte, which gives bytecode its name.[^1] The Java Virtual Machine is an example. Its instructions are "a one-byte opcode specifying the operation to be performed, followed by zero or more operands".[^5]

Bytecode sits between the two other ways to run a program. It keeps the portability of a tree-walker, because the virtual machine is an ordinary program. It is faster than a tree-walker, because the code is dense and linear, but it is slower than native machine code, because the emulation adds a layer.[^1]

## How it works

### A stack machine

The bytecodes of the JVM and of .NET are designed for a stack machine. An instruction takes its operands from the top of a stack of values and pushes its result back. The JVM gives each method frame its own operand stack. Its `iadd` instruction pops two `int` values, adds them, and pushes the sum.[^4] The CIL of .NET works the same way: `add` pops two values and pushes the result onto the evaluation stack.[^11]

An expression compiles to a stack machine in post-order. The compiler emits the code for the left operand, then for the right operand, then the operator. When the operator runs, the right operand is on top of the stack, so the first value it pops is the right one.[^13] `a * b + c` becomes `LOAD a`, `LOAD b`, `MUL`, `LOAD c`, `ADD`. No instruction names a temporary value, which keeps the instructions short.

### Variables become slot numbers

A tree-walker looks up a variable by name in a chain of environments each time it runs. A bytecode compiler can resolve each name once, at compile time, to a slot number in the frame. The JVM records the number of local variable slots of each method, and the maximum depth of its operand stack, in the `Code` attribute of the class file.[^6]

### Jumps replace nested statements

A `while` loop has no nesting in bytecode. The compiler emits the condition, a conditional jump past the body, the body, and an unconditional jump back to the condition. The address of the exit is unknown when the compiler emits the conditional jump, so it writes a placeholder and patches it after the body.

### The dispatch loop

The virtual machine is a loop. The JVM specification describes its inner loop in three steps: fetch an opcode, fetch its operands if it has any, and execute the action for the opcode.[^5] The program below is the whole pipeline: a compiler from a tree to bytecode, a disassembler, and the loop. The source program is the factorial loop that the [[tree-walking-interpreter]] page runs.

```python run id=vm
from enum import IntEnum


class Op(IntEnum):
    PUSH = 0
    LOAD = 1
    STORE = 2
    ADD = 3
    SUB = 4
    MUL = 5
    LT = 6
    JUMP_IF_FALSE = 7
    JUMP = 8
    PRINT = 9
    HALT = 10


HAS_ARG = {Op.PUSH, Op.LOAD, Op.STORE, Op.JUMP_IF_FALSE, Op.JUMP}
BINARY = {"+": Op.ADD, "-": Op.SUB, "*": Op.MUL, "<": Op.LT}


class Compiler:
    """Turns a tree of tuples into a flat list of numbers."""

    def __init__(self):
        self.code = []
        self.slots = {}  # variable name -> slot number, fixed at compile time

    def emit(self, op, arg=None):
        self.code.append(op)
        if arg is not None:
            self.code.append(arg)
        return len(self.code) - 1  # position of the last byte, used for patching

    def expr(self, node):
        match node:
            case ("num", value):
                self.emit(Op.PUSH, value)
            case ("var", name):
                self.emit(Op.LOAD, self.slots[name])
            case (op, left, right):
                self.expr(left)
                self.expr(right)
                self.emit(BINARY[op])

    def stmt(self, node):
        match node:
            case ("let", name, init):
                self.expr(init)
                slot = self.slots.setdefault(name, len(self.slots))
                self.emit(Op.STORE, slot)
            case ("set", name, value):
                self.expr(value)
                self.emit(Op.STORE, self.slots[name])
            case ("block", *body):
                for s in body:
                    self.stmt(s)
            case ("while", cond, body):
                start = len(self.code)
                self.expr(cond)
                hole = self.emit(Op.JUMP_IF_FALSE, 0)
                self.stmt(body)
                self.emit(Op.JUMP, start)
                self.code[hole] = len(self.code)  # patch the exit address
            case ("print", value):
                self.expr(value)
                self.emit(Op.PRINT)


def disassemble(code):
    pc = 0
    while pc < len(code):
        op = Op(code[pc])
        if op in HAS_ARG:
            print(f"{pc:4} {op.name:<13} {code[pc + 1]}")
            pc += 2
        else:
            print(f"{pc:4} {op.name}")
            pc += 1


def run(code, nslots):
    stack, slots, pc = [], [0] * nslots, 0
    executed = max_depth = 0
    while True:
        op = code[pc]
        executed += 1
        if op in HAS_ARG:
            arg = code[pc + 1]
            pc += 2
        else:
            pc += 1
        match op:
            case Op.PUSH:
                stack.append(arg)
            case Op.LOAD:
                stack.append(slots[arg])
            case Op.STORE:
                slots[arg] = stack.pop()
            case Op.ADD | Op.SUB | Op.MUL | Op.LT:
                b = stack.pop()  # the right operand is on top
                a = stack.pop()
                match op:
                    case Op.ADD:
                        stack.append(a + b)
                    case Op.SUB:
                        stack.append(a - b)
                    case Op.MUL:
                        stack.append(a * b)
                    case Op.LT:
                        stack.append(1 if a < b else 0)
            case Op.JUMP_IF_FALSE:
                if stack.pop() == 0:
                    pc = arg
            case Op.JUMP:
                pc = arg
            case Op.PRINT:
                print(stack.pop())
            case Op.HALT:
                return executed, max_depth
        max_depth = max(max_depth, len(stack))


# let n = 5; let acc = 1;
# while (0 < n) { let step = n; acc = acc * step; n = n - 1; }
# print acc;
program = ("block",
    ("let", "n", ("num", 5)),
    ("let", "acc", ("num", 1)),
    ("while", ("<", ("num", 0), ("var", "n")), ("block",
        ("let", "step", ("var", "n")),
        ("set", "acc", ("*", ("var", "acc"), ("var", "step"))),
        ("set", "n", ("-", ("var", "n"), ("num", 1))),
    )),
    ("print", ("var", "acc")),
)

compiler = Compiler()
compiler.stmt(program)
compiler.emit(Op.HALT)
disassemble(compiler.code)
executed, depth = run(compiler.code, len(compiler.slots))
print(f"bytes of code: {len(compiler.code)}")
print(f"instructions executed: {executed}, maximum stack depth: {depth}")
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

The tree-walker on the other page visits 81 nodes for this program. The virtual machine executes a similar number of instructions. The gain is not in the count of steps but in their cost. Each instruction is a read from an array and one branch of the dispatch, with no recursive call. The tree-walker made 37 lookups by name that followed 25 parent links. The virtual machine makes no lookups by name, because the compiler replaced each name by a slot number.

## In languages

Some languages are defined, or always implemented, in terms of a bytecode, and their tools show it. In the others, a program that wants a bytecode builds one, as the examples do. Each example encodes the factorial program above as an array of numbers, disassembles it, runs it, and prints the same lines.

| Language | Verdict | Form |
|---|---|---|
| Java | native | `javac` writes JVM bytecode into class files; `javap -c` disassembles them |
| Kotlin | native | the JVM target writes the same class files as Java |
| C# | native | the compiler writes CIL; `ildasm` disassembles it; `MethodBody.GetILAsByteArray` reads it at run time |
| Python | stdlib | CPython compiles each function to bytecode; the `dis` module disassembles it |
| JavaScript | idiom | the language defines no bytecode; V8 compiles functions to an internal register bytecode |
| TypeScript | idiom | as JavaScript; the type checker emits JavaScript, not bytecode |
| Go | idiom | compiles to machine code; a VM is an ordinary program |
| Rust | idiom | compiles to machine code; an `enum` of instructions or a byte array |
| C | idiom | compiles to machine code; a `switch` inside a loop |
| C++ | idiom | compiles to machine code; the same loop as C |
| PowerShell | idiom | a VM written as a script (no example: pwsh is not installed) |
| Bash | idiom | possible with arrays and a `case` statement, but slow |
| SQL | n/a | a query language, not a host for a VM |
| Regex | n/a | a pattern language |
| HTML/CSS | n/a | no execution |

**JVM.** The JVM "knows nothing of the Java programming language, only of a particular binary format, the class file format". A class file holds JVM instructions, which the specification also calls bytecodes, and a symbol table. Any language that can be expressed as a valid class file can run on the JVM, which is how Kotlin runs there.[^3] Every class file starts with the magic number `0xCAFEBABE`.[^18] `javap` disassembles class files, and its option `-c` prints the instructions of each method.[^8] The Java example compiles a small method at run time and calls `javap` on it.

**CPython.** CPython compiles every function to bytecode, and the `dis` module disassembles it. The documentation warns that bytecode "is an implementation detail of the CPython interpreter" and that instructions may be added, removed or changed between versions. Each instruction takes 2 bytes. The interpreter also adapts the bytecode at run time to specialize it, and `dis` shows the specialized form with `adaptive=True`.[^9] The Python example disassembles a function with `dis`; its output depends on the version of CPython that ran it.

**.NET.** A .NET compiler translates source code into common intermediate language (CIL), "a CPU-independent set of instructions". CIL has instructions for loads, stores, calls, arithmetic, control flow and exception handling. Before it runs, a just-in-time compiler usually converts it to native code.[^12] `Ildasm.exe` takes a portable executable file that contains IL and writes a text file that the IL assembler can read. It is installed with Visual Studio.[^10] A program can read the IL of its own methods with `MethodBody.GetILAsByteArray`.[^17] The C# example does this.

**V8.** V8's Ignition interpreter compiles JavaScript functions to a bytecode that is between 50% and 25% of the size of the equivalent baseline machine code. Ignition is a register machine with an accumulator, not a stack machine.[^16]

## Variations and related ideas

- **Register machines.** In a register VM, an instruction names the slots of its inputs and its output. `c = a + b` becomes one instruction instead of four, with no pushes and pops. Each instruction is larger and harder to decode. Crafting Interpreters reports that Lua switched from a stack to a register instruction set for Lua 5.0 and noted a speed improvement. The book stays with a stack machine because register VMs are harder to write a compiler for.[^14]
- **Measured tradeoff.** Shi, Gregg, Beatty and Ertl translated JVM stack code into register code. The register form eliminated more than 47% of the executed VM instructions, and its code was about 25% larger. On a Pentium 4, the register machine took about 32.3% less time with a C `switch` for dispatch, and about 26.5% less with the more efficient threaded dispatch.[^15]
- **Verification.** A VM that loads code it did not produce must check it. The JVM verifies a class file before it runs it. After verification it knows, among other things, that there are no operand stack overflows or underflows.[^7]
- **Just-in-time compilation.** A VM can translate hot bytecode into machine code while the program runs. CIL is usually run this way.[^12] See [[just-in-time-compilation]].

## Pitfalls

### Popping the operands in the wrong order

The compiler pushes the left operand first, so the right operand is on top. A VM that pops the left operand first swaps the operands of every operator that is not commutative.[^13] Addition and multiplication still give the right answers, so tests with only those operators do not find the bug.

```python wrong id=operand-order
PUSH, SUB, PRINT, HALT = range(4)


def run(code):
    stack, pc = [], 0
    while True:
        op = code[pc]
        pc += 1
        if op == PUSH:
            stack.append(code[pc])
            pc += 1
        elif op == SUB:
            a = stack.pop()  # takes the right operand as the left one
            b = stack.pop()
            stack.append(a - b)
        elif op == PRINT:
            print(stack.pop())
        elif op == HALT:
            return


# 10 - 3: the compiler pushes the left operand first.
run([PUSH, 10, PUSH, 3, SUB, PRINT, HALT])
```

```output for=operand-order
-7
```

`10 - 3` should be 7. The fix pops `b`, the right operand, first.

```python run id=operand-order-fix
PUSH, SUB, PRINT, HALT = range(4)


def run(code):
    stack, pc = [], 0
    while True:
        op = code[pc]
        pc += 1
        if op == PUSH:
            stack.append(code[pc])
            pc += 1
        elif op == SUB:
            b = stack.pop()  # the right operand is on top
            a = stack.pop()
            stack.append(a - b)
        elif op == PRINT:
            print(stack.pop())
        elif op == HALT:
            return


# 10 - 3: the compiler pushes the left operand first.
run([PUSH, 10, PUSH, 3, SUB, PRINT, HALT])
```

```output for=operand-order-fix
7
```

### Trusting the bytecode

The VM assumes that the code was produced by a correct compiler. Bad code makes it pop from an empty stack. In Python that is an exception. In a VM written in C it reads memory below the stack, with no error at all.

```python fails expect=exception:IndexError id=underflow
PUSH, ADD, PRINT, HALT = range(4)


def run(code):
    stack, pc = [], 0
    while True:
        op = code[pc]
        pc += 1
        if op == PUSH:
            stack.append(code[pc])
            pc += 1
        elif op == ADD:
            b = stack.pop()
            a = stack.pop()
            stack.append(a + b)
        elif op == PRINT:
            print(stack.pop())
        elif op == HALT:
            return


# A broken compiler forgot to push the left operand of 2 + 3.
run([PUSH, 3, ADD, PRINT, HALT])
```

The fix checks the code before it runs. The verifier below walks the instructions, tracks the depth of the stack, and rejects the code at the first instruction that would pop more values than the stack holds. It handles straight-line code only. A verifier for code with jumps must follow every path and check that the depths agree where paths meet, as the JVM's verifier does.[^7]

```python run id=underflow-fix
PUSH, ADD, PRINT, HALT = range(4)
EFFECT = {PUSH: (0, 1), ADD: (2, 1), PRINT: (1, 0), HALT: (0, 0)}  # (pops, pushes)


def verify(code):
    """Check the stack depth of every instruction before anything runs."""
    depth, pc = 0, 0
    while pc < len(code):
        op = code[pc]
        pops, pushes = EFFECT[op]
        if depth < pops:
            raise ValueError(f"stack underflow at offset {pc}")
        depth += pushes - pops
        pc += 2 if op == PUSH else 1
    return depth


for code in ([PUSH, 2, PUSH, 3, ADD, PRINT, HALT], [PUSH, 3, ADD, PRINT, HALT]):
    try:
        verify(code)
        print("accepted")
    except ValueError as e:
        print(f"rejected: {e}")
```

```output for=underflow-fix
accepted
rejected: stack underflow at offset 2
```

### Reading CPython bytecode as a stable format

Tools that read `.pyc` files or code objects break when CPython changes its instructions. The `dis` documentation says that no guarantee is made that bytecode will not be added, removed or changed between versions of Python.[^9] Use `dis` to learn and to debug. A tool that must work across versions should not depend on the exact instructions.

## Cost

- **Size.** Bytecode is dense. The example program is 39 bytes, against 22 objects with pointers for the tree. V8 reports that its bytecode is 25% to 50% of the size of baseline machine code.[^16]
- **Dispatch.** Each executed instruction costs one fetch and one indirect branch in the dispatch loop. A stack machine executes more instructions than a register machine for the same work, so it pays this cost more often.[^15]
- **Locality.** The instructions sit next to each other in memory, in the order that they run, which suits the CPU caches.[^1][^2]
- **Compile step.** The compiler must walk the tree once before the first instruction runs.
- **Stack depth.** The compiler knows the maximum depth of the operand stack of each method. The JVM determines it at compile time and stores it in the class file with the code of the method.[^4][^6] The example needs a depth of 2.

## History

- **2005.** Yunhe Shi, David Gregg, Andrew Beatty and M. Anton Ertl published "Virtual Machine Showdown: Stack Versus Registers" at VEE '05. They measured that register code eliminated more than 47% of executed VM instructions, at a cost of about 25% more code.[^15]
- **2016.** The V8 team described Ignition, an interpreter for a register bytecode with an accumulator, built to execute code with less memory overhead than V8's baseline compiler.[^16]

## References
