```csharp run id=local-functions-and-partial-members
// A local function can be called before its declaration in the block.
Console.WriteLine($"Square(5) = {Square(5)}");
static int Square(int x) => x * x;

// An out variable is declared in the call.
if (int.TryParse("42", out var n))
{
    Console.WriteLine($"n = {n}");
}

var p = new Shape();
Console.WriteLine($"p.Name() = {p.Name()}");

// A partial class has two declarations, and the compiler joins them.
partial class Shape
{
    public partial string Name();  // a declaration without a body
}

partial class Shape
{
    public partial string Name() => "shape";  // the definition
}
```

```output for=local-functions-and-partial-members
Square(5) = 25
n = 42
p.Name() = shape
```

- Verdict: native. A local function can be called before its declaration. `out var` declares a variable inside a call. A `partial` method has a declaring declaration without a body and an implementing declaration with the body, and the compiler joins the two parts of the class.
