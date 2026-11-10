```csharp run id=hglobal
using System;
using System.Runtime.InteropServices;

const int Count = 4;
IntPtr totals = Marshal.AllocHGlobal(Count * sizeof(int));
try
{
    for (int i = 0; i < Count; i++)
    {
        Marshal.WriteInt32(totals, i * sizeof(int), i * 10);
    }
    Console.WriteLine($"last total: {Marshal.ReadInt32(totals, (Count - 1) * sizeof(int))}");
}
finally
{
    Marshal.FreeHGlobal(totals); // the garbage collector never frees this block
}
Console.WriteLine("freed");
```

```output
last total: 30
freed
```

- Verdict: stdlib. Objects on the managed heap are freed by the garbage collector; `Marshal` and `NativeMemory` allocate and free memory outside it.
- `AllocHGlobal` does not fill the block with zeros. The program writes each value before it reads it.
- The `finally` block frees the memory on every exit, also when an exception leaves the `try` block.
- The documentation calls `AllocHGlobal` a legacy API and recommends `NativeMemory.Alloc` and `NativeMemory.Free`. They return a raw pointer and need unsafe code, which this example does not turn on.
