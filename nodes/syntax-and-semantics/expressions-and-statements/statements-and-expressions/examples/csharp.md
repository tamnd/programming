```csharp run id=switch-expression-and-null-assignment
static string Label(int score) => score switch
{
    >= 90 => "A",
    >= 50 => "pass",
    _ => "fail",
};

Console.WriteLine(string.Join(" ", new[] { 95, 70, 30 }.Select(Label)));

int a, b;
a = b = 5;
Console.WriteLine($"{a} {b}");

string? name = null;
name ??= "guest"; // assign only when null; the expression has the value
Console.WriteLine(name);
Console.WriteLine((name.Length > 3 ? "long" : "short") + " name");
```

```output for=switch-expression-and-null-assignment
A pass fail
5 5
guest
long name
```

- Verdict: native. C# has the statements and expressions of C and adds expression forms: the `switch` expression with patterns, expression-bodied members with `=>`, `??` and `??=` for null, and `throw` as an expression. Assignment, including `??=`, is an expression whose value is the assigned value.
