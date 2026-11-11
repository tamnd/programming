```csharp run id=machine-in-int-array
using System;

const int HALT = 0, LOAD = 1, ADD = 2, SUB = 3, STORE = 4, JZ = 5, JMP = 6;

int Run(int[] mem)
{
    int pc = 0, acc = 0, steps = 0;
    while (true)
    {
        int op = mem[pc], addr = mem[pc + 1];
        pc += 2;
        steps++;
        switch (op)
        {
            case HALT: return steps;
            case LOAD: acc = mem[addr]; break;
            case ADD: acc += mem[addr]; break;
            case SUB: acc -= mem[addr]; break;
            case STORE: mem[addr] = acc; break;
            case JZ: if (acc == 0) pc = addr; break;
            case JMP: pc = addr; break;
            default: throw new InvalidOperationException($"bad opcode {op}");
        }
    }
}

int[] mem = [LOAD, 20, JZ, 18, LOAD, 21, ADD, 20, STORE, 21,
             LOAD, 20, SUB, 22, STORE, 20, JMP, 0, HALT, 0, 5, 0, 1];
int steps = Run(mem);
Console.WriteLine($"sum={mem[21]} steps={steps}");
Console.WriteLine($"{IntPtr.Size} {sizeof(long)} {BitConverter.IsLittleEndian}");
```

```output for=machine-in-int-array
sum=15 steps=48
8 8 True
```

- Verdict: native. C# runs on the .NET runtime, which compiles its bytecode to machine code. Types such as `int` and `long` have fixed widths, while `IntPtr.Size` gives the size of an address on the machine, 8 bytes on a 64-bit processor. `unsafe` code can use pointers when a program needs the machine directly.
