```csharp run id=for-foreach-while-do-and-linq
using System.Linq;

int sum = 0;
for (int i = 1; i <= 5; i++)
{
    sum += i;
}
Console.WriteLine($"for: {sum}");

foreach (var name in new[] { "ann", "bob" })
{
    Console.WriteLine($"foreach: {name}");
}

int n = 100;
int steps = 0;
while (n > 1)
{
    n /= 2;
    steps++;
}
Console.WriteLine($"while: {steps}");

// The do loop runs its body one or more times.
int runs = 0;
do
{
    runs++;
} while (runs < 0);
Console.WriteLine($"do: {runs}");

// LINQ replaces a loop that filters and sums.
int evens = Enumerable.Range(1, 10).Where(x => x % 2 == 0).Sum();
Console.WriteLine($"linq: {evens}");
```

```output for=for-foreach-while-do-and-linq
for: 15
foreach: ann
foreach: bob
while: 6
do: 1
linq: 30
```

- Verdict: native. C# has four iteration statements: `for`, `foreach`, `while` and `do`. The `do` body runs once although its condition is false. `Enumerable.Range(1, 10).Where(...).Sum()` adds the even numbers 2, 4, 6, 8 and 10 with no loop statement.
