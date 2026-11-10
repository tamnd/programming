```csharp fails expect=exception:ArgumentOutOfRangeException id=as-span
static ReadOnlySpan<byte> Echo(byte[] payload, int claimed) => payload.AsSpan(0, claimed);

byte[] payload = "hello"u8.ToArray();
Console.WriteLine(Echo(payload, 5).Length);
Console.WriteLine(Echo(payload, 64).Length);
```

`AsSpan(0, 64)` on an array of 5 bytes throws `ArgumentOutOfRangeException`, because the range is not inside the array.

```csharp run id=check
using System.Text;

static bool TryEcho(byte[] payload, int claimed, out ReadOnlySpan<byte> reply)
{
    if (claimed < 0 || claimed > payload.Length)
    {
        reply = default;
        return false;
    }
    reply = payload.AsSpan(0, claimed);
    return true;
}

byte[] payload = "hello"u8.ToArray();
foreach (int claimed in new[] { 5, 64 })
{
    if (TryEcho(payload, claimed, out var reply))
    {
        Console.WriteLine($"claimed {claimed}: {Encoding.ASCII.GetString(reply)}");
    }
    else
    {
        Console.WriteLine($"claimed {claimed}: discarded");
    }
}
```

```output for=check
claimed 5: hello
claimed 64: discarded
```

- Verdict: native. Array, span and string accesses are checked, and a range out of bounds throws an exception.
- An unhandled exception is a denial of service. Check lengths from input and return a result instead.
- `unsafe` code with pointers, `stackalloc` and `Unsafe` methods can read out of bounds.
