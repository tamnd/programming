```csharp run id=engine
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;

var f = new JitFunction("f", new Bin('+', new Bin('*', new Arg(), new Arg()), new Num(1)));
Value[] inputs = [new IntV(1), new IntV(2), new IntV(3), new IntV(4), new FloatV(2.5), new IntV(6), new IntV(7), new IntV(8)];
foreach (var arg in inputs)
{
    var (result, tier) = f.Call(arg);
    Console.WriteLine($"f({arg}) = {result} [{tier}]");
}
Console.WriteLine(string.Join(", ", f.Counts.Select(kv => $"{kv.Key}: {kv.Value}")));

// Values carry their type, as boxed values do in a dynamic language.
abstract record Value;
record IntV(long N) : Value { public override string ToString() => N.ToString(CultureInfo.InvariantCulture); }
record FloatV(double X) : Value { public override string ToString() => X.ToString(CultureInfo.InvariantCulture); }

// The body of f(x) = x * x + 1 as a tree.
abstract record Node;
record Num(long Value) : Node;
record Arg : Node;
record Bin(char Op, Node Left, Node Right) : Node;

static class Engine
{
    public static Func<long, long, long> IntOp(char op) => op == '+' ? (a, b) => a + b : (a, b) => a * b;

    static double AsDouble(Value v) => v switch
    {
        IntV i => i.N,
        FloatV x => x.X,
        _ => throw new ArgumentException("unknown value"),
    };

    // Generic arithmetic: check both tags on every operation.
    public static Value Arith(char op, Value a, Value b)
    {
        if (a is IntV x && b is IntV y) return new IntV(IntOp(op)(x.N, y.N));
        double p = AsDouble(a), q = AsDouble(b);
        return new FloatV(op == '+' ? p + q : p * q);
    }

    // The interpreter: walk the tree on every call.
    public static Value Interpret(Node node, Value arg) => node switch
    {
        Num n => new IntV(n.Value),
        Arg => arg,
        Bin b => Arith(b.Op, Interpret(b.Left, arg), Interpret(b.Right, arg)),
        _ => throw new ArgumentException("unknown node"),
    };

    // Compiled code for int: delegates over raw longs, no tag checks inside.
    public static Func<long, long> CompileInt(Node node)
    {
        switch (node)
        {
            case Num n: { long v = n.Value; return _ => v; }
            case Arg: return x => x;
            case Bin b:
                var f = IntOp(b.Op);
                var l = CompileInt(b.Left);
                var r = CompileInt(b.Right);
                return x => f(l(x), r(x));
            default: throw new ArgumentException("unknown node");
        }
    }

    // Compiled code for any argument: delegates that still check tags.
    public static Func<Value, Value> CompileGeneric(Node node)
    {
        switch (node)
        {
            case Num n: { Value boxed = new IntV(n.Value); return _ => boxed; }
            case Arg: return a => a;
            case Bin b:
                var l = CompileGeneric(b.Left);
                var r = CompileGeneric(b.Right);
                return a => Arith(b.Op, l(a), r(a));
            default: throw new ArgumentException("unknown node");
        }
    }
}

class JitFunction(string name, Node body)
{
    const int Threshold = 3;
    int calls; // calls in the interpreter since the last compilation
    readonly SortedSet<string> seen = new(StringComparer.Ordinal); // the profile: the kinds of the arguments
    Func<Value, Value?>? code; // compiled code; it returns null when the guard fails
    string kind = "";
    public Dictionary<string, int> Counts { get; } = new() { ["interpreter"] = 0, ["compiled"] = 0, ["deoptimizations"] = 0 };

    void Compile()
    {
        string how;
        if (seen.SetEquals(["int"]))
        {
            var raw = Engine.CompileInt(body);
            code = a => a is IntV i ? new IntV(raw(i.N)) : null; // the guard
            kind = "int";
            how = "specialized for int";
        }
        else
        {
            code = Engine.CompileGeneric(body);
            kind = how = "generic";
        }
        Console.WriteLine($"* compile {name}: arguments seen: {string.Join(", ", seen)}; {how}");
    }

    public (Value, string) Call(Value arg)
    {
        if (code is null && calls >= Threshold) Compile();
        if (code is not null)
        {
            var result = code(arg);
            if (result is not null)
            {
                Counts["compiled"]++;
                return (result, $"compiled, {kind}");
            }
            Console.WriteLine($"* deoptimize {name}: guard failed, {arg} is not an int");
            Counts["deoptimizations"]++;
            code = null;
            calls = 0;
        }
        calls++;
        Counts["interpreter"]++;
        seen.Add(arg is IntV ? "int" : "float");
        return (Engine.Interpret(body, arg), $"interpreter, call {calls}");
    }
}
```

```output for=engine
f(1) = 2 [interpreter, call 1]
f(2) = 5 [interpreter, call 2]
f(3) = 10 [interpreter, call 3]
* compile f: arguments seen: int; specialized for int
f(4) = 17 [compiled, int]
* deoptimize f: guard failed, 2.5 is not an int
f(2.5) = 7.25 [interpreter, call 1]
f(6) = 37 [interpreter, call 2]
f(7) = 50 [interpreter, call 3]
* compile f: arguments seen: float, int; generic
f(8) = 65 [compiled, generic]
interpreter: 6, compiled: 2, deoptimizations: 1
```

- Verdict: native. The .NET runtime compiles CIL to native code just in time. With tiered compilation, which is on by default, a method gets quick code first and optimized code later, generated in the background.
- `a is IntV i` is the guard. The pattern tests the type and binds the unboxed value in one step.
- `Func<long, long>` delegates over `long` values keep the compiled code for `int` free of boxing.
- The records print their numbers with `CultureInfo.InvariantCulture`, so `2.5` prints the same way under every culture setting of the machine.
