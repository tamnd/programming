```csharp run id=vm
using System;
using System.Collections.Generic;

// let n = 5; let acc = 1; while (0 < n) { let step = n; acc = acc * step; n = n - 1; } print acc;
// Slots: n = 0, acc = 1, step = 2.
byte[] code = [
    0, 5, 2, 0, 0, 1, 2, 1,
    0, 0, 1, 0, 6, 7, 35,
    1, 0, 2, 2,
    1, 1, 1, 2, 5, 2, 1,
    1, 0, 0, 1, 4, 2, 0,
    8, 8,
    1, 1, 9, 10,
];

static bool HasArg(Op op) => op is Op.PUSH or Op.LOAD or Op.STORE or Op.JUMP_IF_FALSE or Op.JUMP;

for (int at = 0; at < code.Length; )
{
    var op = (Op)code[at];
    if (HasArg(op))
    {
        Console.WriteLine($"{at,4} {op,-13} {code[at + 1]}");
        at += 2;
    }
    else
    {
        Console.WriteLine($"{at,4} {op}");
        at += 1;
    }
}

var stack = new Stack<int>();
var slots = new int[3];
int pc = 0, executed = 0, maxDepth = 0;
bool running = true;
while (running)
{
    var op = (Op)code[pc];
    executed++;
    int arg = HasArg(op) ? code[pc + 1] : 0;
    pc += HasArg(op) ? 2 : 1;
    switch (op)
    {
        case Op.PUSH: stack.Push(arg); break;
        case Op.LOAD: stack.Push(slots[arg]); break;
        case Op.STORE: slots[arg] = stack.Pop(); break;
        case Op.ADD or Op.SUB or Op.MUL or Op.LT:
            int b = stack.Pop(); // the right operand is on top
            int a = stack.Pop();
            stack.Push(op switch
            {
                Op.ADD => a + b,
                Op.SUB => a - b,
                Op.MUL => a * b,
                _ => a < b ? 1 : 0,
            });
            break;
        case Op.JUMP_IF_FALSE: if (stack.Pop() == 0) pc = arg; break;
        case Op.JUMP: pc = arg; break;
        case Op.PRINT: Console.WriteLine(stack.Pop()); break;
        case Op.HALT: running = false; break;
        default: throw new InvalidOperationException($"bad opcode {op}");
    }
    maxDepth = Math.Max(maxDepth, stack.Count);
}
Console.WriteLine($"bytes of code: {code.Length}");
Console.WriteLine($"instructions executed: {executed}, maximum stack depth: {maxDepth}");

enum Op : byte { PUSH, LOAD, STORE, ADD, SUB, MUL, LT, JUMP_IF_FALSE, JUMP, PRINT, HALT }
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

The second program reads the CIL of one of its own methods with `MethodBody.GetILAsByteArray` and decodes it with the `OpCodes` table from `System.Reflection.Emit`. The exact instructions depend on the compiler and on whether it optimized the build.

```csharp run id=il
using System;
using System.Collections.Generic;
using System.Reflection;
using System.Reflection.Emit;

var oneByte = new Dictionary<byte, OpCode>();
var twoByte = new Dictionary<byte, OpCode>();
foreach (var field in typeof(OpCodes).GetFields(BindingFlags.Public | BindingFlags.Static))
{
    var opcode = (OpCode)field.GetValue(null)!;
    if (opcode.Size == 1) oneByte[(byte)opcode.Value] = opcode;
    else twoByte[(byte)(opcode.Value & 0xFF)] = opcode;
}

MethodInfo method = typeof(Fact).GetMethod(nameof(Fact.Compute))!;
byte[] il = method.GetMethodBody()!.GetILAsByteArray()!;
Console.WriteLine($"Fact.Compute(5) = {Fact.Compute(5)}");
Console.WriteLine($"bytes of IL: {il.Length}");

for (int pc = 0; pc < il.Length; )
{
    int start = pc;
    OpCode op = il[pc] == 0xFE ? twoByte[il[pc + 1]] : oneByte[il[pc]];
    pc += op.Size;
    string operand = "";
    switch (op.OperandType)
    {
        case OperandType.InlineNone:
            break;
        case OperandType.ShortInlineI:
            operand = ((sbyte)il[pc]).ToString();
            pc += 1;
            break;
        case OperandType.ShortInlineVar:
            operand = il[pc].ToString();
            pc += 1;
            break;
        case OperandType.ShortInlineBrTarget:
            operand = $"IL_{pc + 1 + (sbyte)il[pc]:X4}";
            pc += 1;
            break;
        case OperandType.InlineBrTarget:
            operand = $"IL_{pc + 4 + BitConverter.ToInt32(il, pc):X4}";
            pc += 4;
            break;
        case OperandType.InlineI:
            operand = BitConverter.ToInt32(il, pc).ToString();
            pc += 4;
            break;
        default:
            throw new NotSupportedException($"operand type {op.OperandType}");
    }
    Console.WriteLine($"IL_{start:X4}: {op.Name} {operand}".TrimEnd());
}

static class Fact
{
    public static int Compute(int n)
    {
        int acc = 1;
        while (0 < n)
        {
            acc = acc * n;
            n = n - 1;
        }
        return acc;
    }
}
```

```output for=il
Fact.Compute(5) = 120
bytes of IL: 30
IL_0000: nop
IL_0001: ldc.i4.1
IL_0002: stloc.0
IL_0003: br.s IL_0010
IL_0005: nop
IL_0006: ldloc.0
IL_0007: ldarg.0
IL_0008: mul
IL_0009: stloc.0
IL_000A: ldarg.0
IL_000B: ldc.i4.1
IL_000C: sub
IL_000D: starg.s 0
IL_000F: nop
IL_0010: ldc.i4.0
IL_0011: ldarg.0
IL_0012: clt
IL_0014: stloc.1
IL_0015: ldloc.1
IL_0016: brtrue.s IL_0005
IL_0018: ldloc.0
IL_0019: stloc.2
IL_001A: br.s IL_001C
IL_001C: ldloc.2
IL_001D: ret
```

- Verdict: native. The C# compiler translates source code to CIL, and the runtime compiles the CIL to native code before it runs.
- CIL is a stack machine, as the decoded method shows. `ldarg.0` pushes the argument, `mul` pops two values and pushes the product, and `stloc.0` pops into local 0.
- Many CIL instructions have short forms. `ldc.i4.1` pushes the constant 1 with no operand byte, and `br.s` takes a one-byte offset. The toy machine spends two bytes on each `PUSH`.
- The VM casts each byte to the `Op` enum. A cast does not check the value, so a byte of 11 becomes an `Op` with no name, and the `default` case throws.
