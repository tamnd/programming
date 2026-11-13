```csharp run id=query-syntax
using System;
using System.Collections.Generic;
using System.Linq;

string[] words = { "apple", "fig", "banana", "kiwi", "cherry" };

// Query syntax: from names the source, where is the filter, select is the expression.
var lengths = from w in words where w.Length > 3 select w.Length;
Console.WriteLine(string.Join(",", lengths));

// Two from clauses nest like two loops.
var pairs = from x in new[] { 1, 2 } from y in new[] { "a", "b" } select $"{x}{y}";
Console.WriteLine(string.Join(",", pairs));

// The compiler turns query syntax into method calls, so these give the same items.
var same = words.Where(w => w.Length > 3).Select(w => w.Length);
Console.WriteLine(lengths.SequenceEqual(same));

// ToHashSet and ToDictionary build other collections.
var firsts = words.Select(w => w[0]).ToHashSet();
Console.WriteLine(string.Join(",", firsts.OrderBy(c => c)));
var withI = words.Where(w => w.Contains('i')).ToDictionary(w => w, w => w.Length);
Console.WriteLine(string.Join(",", withI.Select(p => $"{p.Key}={p.Value}")));

// A query is deferred: it runs again each time a loop reads it.
var list = new List<int> { 1, 2, 3 };
var doubled = from n in list select n * 2;
list.Add(4);
Console.WriteLine(string.Join(",", doubled));
```

```output
5,6,4,6
1a,1b,2a,2b
True
a,b,c,f,k
fig=3,kiwi=4
2,4,6,8
```

- Verdict: native. C# query syntax has from for the source, where for the filter and select for the expression, and two from clauses nest like two loops. The compiler turns the query into Where and Select calls, so the method syntax gives the same items. The last query is deferred: it reads the list when string.Join reads the query, after the program adds 4, so the result has four items.
