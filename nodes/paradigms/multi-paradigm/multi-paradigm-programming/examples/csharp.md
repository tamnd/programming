```csharp run id=orders
using System;
using System.Collections.Generic;
using System.Linq;

string text = "ana, 30\nbo, 70\nana, 25\ncy, 10\nbo, 5";
var totals = new Dictionary<string, int>();
foreach (string line in text.Split('\n'))                 // an imperative loop
{
    Order o = Order.Parse(line);
    totals[o.Customer] = totals.GetValueOrDefault(o.Customer) + o.Amount;
}
var big = totals.Where(kv => kv.Value > 50)               // a LINQ query
                .OrderByDescending(kv => kv.Value)
                .ToList();
foreach (IReport r in new IReport[] { new Plain(), new Csv() })
    foreach (var (name, total) in big)
        Console.WriteLine(r.Line(name, total));

record Order(string Customer, int Amount)                 // a value type
{
    public static Order Parse(string line)                // a pure function
    {
        string[] p = line.Split(',');
        return new Order(p[0].Trim(), int.Parse(p[1]));
    }
}

interface IReport { string Line(string name, int total); }
class Plain : IReport { public string Line(string n, int t) => $"{n} {t}"; }
class Csv : IReport { public string Line(string n, int t) => $"{n},{t}"; }
```

```output for=orders
bo 75
ana 55
bo,75
ana,55
```

```csharp wrong id=for-capture
using System;
using System.Collections.Generic;

// Build three functions that should return 0, 1 and 2.
var fns = new List<Func<int>>();
for (int i = 0; i < 3; i++)
{
    fns.Add(() => i);
}
foreach (var f in fns) Console.Write(f());
Console.WriteLine();
```

```output for=for-capture
333
```

- Verdict: native. C# combines classes and interfaces with lambdas, LINQ, records, pattern matching, `async` and `await`, and top-level statements for procedural scripts. The first block uses a record as a value, a LINQ query, a loop that updates a dictionary, and an interface with two classes.
- The wrong block captures the variable of a `for` loop, which is one variable for the whole loop, so every function returns its last value, 3. A `foreach` loop makes a new variable for each iteration. In a `for` loop, copy the value into a local inside the body: `int j = i; fns.Add(() => j);`.
