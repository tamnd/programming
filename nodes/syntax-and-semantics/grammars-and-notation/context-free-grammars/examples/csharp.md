```csharp run id=balancing-groups
using System;
using System.Text.RegularExpressions;

// Balanced parentheses form a context-free language, not a regular one.
// .NET balancing groups give the regex engine a stack of captures.
var balanced = new Regex(@"^(?:[^()]|(?<open>\()|(?<-open>\)))*(?(open)(?!))$");
// A true regular expression handles only a fixed depth, here two levels.
var depthTwo = new Regex(@"^(?:[^()]|\((?:[^()]|\([^()]*\))*\))*$");

foreach (var s in new[] { "f(a, g(b))", "((x))", "(((x)))", "f(a))", "(()" })
{
    Console.WriteLine($"{s,-11} balancing groups: {balanced.IsMatch(s),-5}  depth-two regex: {depthTwo.IsMatch(s)}");
}
```

```output for=balancing-groups
f(a, g(b))  balancing groups: True   depth-two regex: True
((x))       balancing groups: True   depth-two regex: True
(((x)))     balancing groups: True   depth-two regex: False
f(a))       balancing groups: False  depth-two regex: False
(()         balancing groups: False  depth-two regex: False
```

- Verdict: library. ANTLR, Pidgin, Superpower and Sprache give C# parsers from grammars or combinators, and Roslyn exposes the syntax of C# itself. Balanced parentheses are the standard example of a context-free language that no regular expression can match. The depth-two pattern shows the limit: a true regular expression must spell out each level, so it rejects `(((x)))`. .NET adds balancing groups, which push a capture for each `(` and pop one for each `)`, and the conditional `(?(open)(?!))` fails if any capture remains.
