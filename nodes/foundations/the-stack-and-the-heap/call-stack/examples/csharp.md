```csharp run id=trace
using System;
using System.Diagnostics;
using System.Linq;
using System.Runtime.CompilerServices;

Demo.main();

static class Demo
{
    // The frames of this class on the stack, innermost first, without WhereAmI itself.
    [MethodImpl(MethodImplOptions.NoInlining)]
    static string WhereAmI() =>
        string.Join(" <- ", new StackTrace(1)
            .GetFrames()
            .Select(DiagnosticMethodInfo.Create)
            .Where(m => m?.DeclaringTypeName == "Demo")
            .Select(m => m!.Name));

    [MethodImpl(MethodImplOptions.NoInlining)]
    static void c() => Console.WriteLine("      stack: " + WhereAmI());

    [MethodImpl(MethodImplOptions.NoInlining)]
    static void b()
    {
        Console.WriteLine("    call c");
        c();
        Console.WriteLine("    back in b");
    }

    [MethodImpl(MethodImplOptions.NoInlining)]
    static void a()
    {
        Console.WriteLine("  call b");
        b();
        Console.WriteLine("  back in a");
    }

    [MethodImpl(MethodImplOptions.NoInlining)]
    public static void main()
    {
        Console.WriteLine("call a");
        a();
        Console.WriteLine("back in main");
    }
}
```

```output for=trace
call a
  call b
    call c
      stack: c <- b <- a <- main
    back in b
  back in a
back in main
```

- Verdict: native. `System.Diagnostics.StackTrace` captures the frames of the current thread. `new StackTrace(1)` skips one frame, the frame of `WhereAmI`.
- The JIT compiler can inline a small method, and then its frame is not in the trace. `MethodImplOptions.NoInlining` keeps each frame. The methods have lower-case names only so that the output matches the other files.
