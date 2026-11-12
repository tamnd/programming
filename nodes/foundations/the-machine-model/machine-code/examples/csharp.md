```csharp run id=emit-il
#:property PublishAot=false
using System.Reflection.Emit;

// Write the IL of "static int Add(int a, int b) => a + b" by hand.
var dm = new DynamicMethod("Add", typeof(int), [typeof(int), typeof(int)]);
ILGenerator il = dm.GetILGenerator();
OpCode[] ops = [OpCodes.Ldarg_0, OpCodes.Ldarg_1, OpCodes.Add, OpCodes.Ret];
foreach (OpCode op in ops)
{
    il.Emit(op);
    Console.WriteLine($"  {op.Name,-8} 0x{op.Value:x2} ({op.Size} byte)");
}

// The JIT compiler turns the IL into machine code the first time the method runs.
var add = dm.CreateDelegate<Func<int, int, int>>();
Console.WriteLine($"Add(40, 2) = {add(40, 2)}");
Console.WriteLine($"Add(int.MaxValue, 1) = {add(int.MaxValue, 1)}");
```

```output for=emit-il
  ldarg.0  0x02 (1 byte)
  ldarg.1  0x03 (1 byte)
  add      0x58 (1 byte)
  ret      0x2a (1 byte)
Add(40, 2) = 42
Add(int.MaxValue, 1) = -2147483648
```

- Verdict: absent. C# compiles to CIL, and the JIT compiler or Native AOT makes the machine code. `DynamicMethod` and `ILGenerator` let the program write CIL at run time; the loop prints the name, the byte value and the size of each opcode. The first call compiles the method to machine code. The line `#:property PublishAot=false` is needed in a file-based app, because Native AOT cannot run code that the program writes at run time.
