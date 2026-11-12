```csharp run id=offsets-and-pinning
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

int[] xs = [10, 20, 30, 40];
// Managed references are addresses that the runtime updates when the GC moves an array.
nint d = Unsafe.ByteOffset(ref xs[0], ref xs[2]);
Console.WriteLine($"xs[2] is {d} bytes after xs[0]");

Console.WriteLine($"offset of Count: {Marshal.OffsetOf<Rec>(nameof(Rec.Count))}");
Console.WriteLine($"offset of Tag: {Marshal.OffsetOf<Rec>(nameof(Rec.Tag))}");
Console.WriteLine($"size: {Marshal.SizeOf<Rec>()}");

GCHandle h = GCHandle.Alloc(xs, GCHandleType.Pinned); // pinned: the GC must not move it
nint a0 = h.AddrOfPinnedObject();
Console.WriteLine($"read at address + 12: {Marshal.ReadInt32(a0 + 12)}");
h.Free();

[StructLayout(LayoutKind.Sequential)]
struct Rec
{
    public byte Flag;
    public int Count;
    public byte Tag;
}
```

```output for=offsets-and-pinning
xs[2] is 8 bytes after xs[0]
offset of Count: 4
offset of Tag: 8
size: 12
read at address + 12: 40
```

- Verdict: native. A `ref` is a managed pointer that the runtime updates when the garbage collector moves the array, and `Unsafe.ByteOffset` gives the distance between two of them. `Marshal.OffsetOf` and `Marshal.SizeOf` give the layout of a struct with sequential layout, which has padding before `Count`. A pinned `GCHandle` stops the collector from moving the array, so its address is safe to use until `Free`. C# also has `&`, `*` and pointer arithmetic in `unsafe` code.
