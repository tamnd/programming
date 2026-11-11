```csharp run id=extension-methods
// Cases are records; operations are extension methods with a switch.
var e = new Add(new Num(1), new Neg(new Add(new Num(2), new Num(3))));
Console.WriteLine($"{e.Show()} = {e.Evaluate()}");

public abstract record Expr;
public record Num(int Value) : Expr;
public record Add(Expr Left, Expr Right) : Expr;
public record Neg(Expr Inner) : Expr;

public static class Eval
{
    public static int Evaluate(this Expr e) => e switch
    {
        Num n => n.Value,
        Add a => a.Left.Evaluate() + a.Right.Evaluate(),
        Neg g => -g.Inner.Evaluate(),
        _ => throw new ArgumentException($"no case for {e.GetType().Name}"),
    };
}

// A new operation in another class: the records do not change.
public static class Printing
{
    public static string Show(this Expr e) => e switch
    {
        Num n => n.Value.ToString(),
        Add a => $"({a.Left.Show()} + {a.Right.Show()})",
        Neg g => $"-{g.Inner.Show()}",
        _ => throw new ArgumentException($"no case for {e.GetType().Name}"),
    };
}
```

```output for=extension-methods
(1 + -(2 + 3)) = -4
```

```csharp wrong id=static-overload
Expr e = new Mul(new Num(2), new Num(3));
Console.WriteLine(e.Show());
Console.WriteLine(new Mul(new Num(2), new Num(3)).Show());

public abstract record Expr;
public record Num(int Value) : Expr;
public record Mul(Expr Left, Expr Right) : Expr;  // a case added later

public static class Printing
{
    public static string Show(this Expr e) => e switch
    {
        Num n => n.Value.ToString(),
        _ => $"<{e.GetType().Name}?>",
    };
}

// The new case adds its own overload instead of a switch arm.
public static class MulPrinting
{
    public static string Show(this Mul m) => $"({m.Left.Show()} * {m.Right.Show()})";
}
```

```output for=static-overload
<Mul?>
(2 * 3)
```

- Verdict: idiom. Extension methods let a class in any assembly add an operation that reads as a method of the existing types, and a `switch` expression with type patterns selects the case. Records give short case types. The compiler warns when a `switch` expression on a type does not cover every value, and the discard arm `_` turns a missing case into an exception at run time.
- The wrong block adds a case with its own overload of the extension method. Extension methods bind at compile time by the static type of the receiver, so a call through a variable of type `Expr` picks `Show(this Expr)`, and the overload for `Mul` runs only when the static type is `Mul`. A new case must add an arm to the `switch`, or the operation must be a virtual method of the case types.
