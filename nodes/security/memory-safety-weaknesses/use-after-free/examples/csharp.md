```csharp fails expect=exception:ObjectDisposedException id=disposed
using System.Text;

var stream = new MemoryStream();
stream.Write(Encoding.UTF8.GetBytes("alice\n"));
stream.Dispose();
stream.Write(Encoding.UTF8.GetBytes("bob\n"));
```

```csharp run id=using
using System.Text;

static void WriteSession()
{
    using var stream = new MemoryStream();
    stream.Write(Encoding.UTF8.GetBytes("alice\n"));
    Console.WriteLine($"wrote {stream.Length} bytes");
} // stream is disposed here, and its name is out of scope

WriteSession();
```

```output for=using
wrote 6 bytes
```

- Verdict: native. The garbage collector frees an object only when no reference to it is left, so safe C# code cannot use freed memory.
- The bug moves to resources that implement `IDisposable`. An operation on a disposed object throws `ObjectDisposedException`.
- The C# documentation says to declare a disposable variable in the `using` statement or with a `using` declaration, because an instance that stays in scope after it is disposed can still be used.
- `unsafe` code with pointers, `Marshal.FreeHGlobal` and native code through P/Invoke manage memory by hand and have the same weakness as C.
