```csharp run id=tuple-switch-expression
using System.Text.RegularExpressions;

static S Step(S s, char ch)
{
    bool digit = char.IsAsciiDigit(ch);
    return (s, ch) switch
    {
        (S.Start, '+' or '-') => S.Sign,
        (S.Start or S.Sign or S.Int, _) when digit => S.Int,
        (S.Int, '.') => S.Dot,
        (S.Dot or S.Frac, _) when digit => S.Frac,
        _ => S.Dead,
    };
}

static bool Accepts(string text)
{
    var s = S.Start;
    foreach (var ch in text) s = Step(s, ch);
    return s is S.Int or S.Frac;
}

string[] inputs = ["42", "-3.5", "1.", "+", "007", ".5", "4.2.1"];
foreach (var t in inputs) Console.WriteLine($"{t} {Accepts(t)}");
var pattern = new Regex(@"^[+-]?[0-9]+(\.[0-9]+)?$");
Console.WriteLine($"regex agrees: {inputs.All(t => pattern.IsMatch(t) == Accepts(t))}");

enum S { Start, Sign, Int, Dot, Frac, Dead }
```

```output for=tuple-switch-expression
42 True
-3.5 True
1. False
+ False
007 True
.5 False
4.2.1 False
regex agrees: True
```

- Verdict: idiom. A C# `enum` names the states, and a `switch` expression on the pair of state and character, with `or` patterns and `when` guards, reads like the transition table. The compiler warns when a switch expression does not cover every input, and the last arm `_` sends the rest to the dead state.
