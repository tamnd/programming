```csharp run id=defined
int x = int.MaxValue;
Console.WriteLine(unchecked(x + 1));
try
{
    Console.WriteLine(checked(x + 1));
}
catch (OverflowException e)
{
    Console.WriteLine(e.Message);
}

int[] a = [1, 2, 3];
int i = 3;
try
{
    Console.WriteLine(a[i]);
}
catch (IndexOutOfRangeException e)
{
    Console.WriteLine(e.Message);
}
```

```output for=defined
-2147483648
Arithmetic operation resulted in an overflow.
Index was outside the bounds of the array.
```

```csharp fails expect=compile id=unsafe-off
int x = 5;
unsafe
{
    int* p = &x;
    Console.WriteLine(*p);
}
```

The project does not allow unsafe code, so the compiler rejects the `unsafe` block with error CS0227.

```csharp run id=unsafe-on
#:property AllowUnsafeBlocks=true

int x = 5;
unsafe
{
    int* p = &x;
    Console.WriteLine(*p);
}
```

```output for=unsafe-on
5
```

- Verdict: native. Safe C# gives a defined result or an exception for each operation. The C# specification uses the words "undefined behavior" for unsafe code, for example for the dereference of an invalid pointer.
- Integer arithmetic is unchecked by default and wraps. `checked` turns overflow into an `OverflowException`.
- Unsafe code needs the `unsafe` keyword and the `AllowUnsafeBlocks` project property. The file-based app sets the property with the `#:property` directive.
