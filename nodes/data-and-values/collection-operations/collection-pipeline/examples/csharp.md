```csharp run id=linq-method-chain
using System;
using System.Collections.Generic;
using System.Linq;

var articles = new[]
{
    (Title: "Intro to NoSQL", Words: 1200, Tags: new[] { "nosql", "databases" }),
    (Title: "Event sourcing", Words: 800, Tags: new[] { "architecture" }),
    (Title: "Graph stores", Words: 1500, Tags: new[] { "nosql", "graphs" }),
    (Title: "Refactoring loops", Words: 600, Tags: new[] { "refactoring" }),
    (Title: "Key-value stores", Words: 900, Tags: new[] { "nosql" }),
};

// Where, OrderByDescending, Take and Select: a LINQ method chain.
var nosql = articles.Where(a => a.Tags.Contains("nosql")).ToList();
Console.WriteLine(string.Join(", ", nosql.OrderByDescending(a => a.Words).Take(2).Select(a => a.Title)));

// Select and Aggregate are map and reduce.
Console.WriteLine(nosql.Select(a => a.Words).Aggregate(0, (acc, n) => acc + n));

// SelectMany and GroupBy: the number of articles for each tag.
var counts = articles
    .SelectMany(a => a.Tags)
    .GroupBy(t => t)
    .OrderBy(g => g.Key, StringComparer.Ordinal)
    .Select(g => $"{g.Key}={g.Count()}");
Console.WriteLine(string.Join(" ", counts));

int calls = 0;
int Score((string Title, int Words, string[] Tags) a)
{
    calls++;
    return a.Words;
}

// ToList in the middle makes the pipeline eager: every score is computed.
var eager = articles.Select(Score).ToList().Where(s => s > 1000).Take(1).ToList();
Console.WriteLine($"eager {string.Join(",", eager)} {calls}");

// A LINQ chain is deferred: Take(1) stops the work after the first match.
calls = 0;
var lazy = articles.Select(Score).Where(s => s > 1000).Take(1).ToList();
Console.WriteLine($"lazy {string.Join(",", lazy)} {calls}");
```

```output
Graph stores, Intro to NoSQL
3600
architecture=1 databases=1 graphs=1 nosql=3 refactoring=1
eager 1200 5
lazy 1200 1
```

- Verdict: stdlib. LINQ methods such as Where, OrderByDescending, Take, Select, SelectMany, GroupBy and Aggregate form the chain. A LINQ chain is deferred, so it runs only when ToList or string.Join reads it. ToList between Select and Where makes the pipeline eager, so Score runs 5 times. The chain without it stops after the first match, so Score runs once.
