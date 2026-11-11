```csharp run id=scopes
Console.WriteLine(Caller());

int total = 0;
for (int i = 0; i < 3; i++)
{
    int square = i * i;
    total += square;
}
Console.WriteLine($"total: {total}");

static string ReadsField() => Holder.Level;

static string Caller()
{
    string Level = "caller";
    return ReadsField() + ", local " + Level;
}

static class Holder
{
    public static string Level = "class";
}
```

```output for=scopes
class, local caller
total: 5
```

- Verdict: native. A local variable is in scope in its block, and the blocks of a method nest as in Java.
- A local variable cannot have the name of another local variable in an enclosing or nested block. The compiler reports error CS0136, whatever the order of the two declarations in the text.

```csharp fails expect=compile id=cs0136
int count = 3;
for (int i = 0; i < 2; i++)
{
    int count = i;
    Console.WriteLine(count);
}
Console.WriteLine(count);
```

The inner `count` would hide the outer `count`, so the compiler rejects the program with CS0136.
