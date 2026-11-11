```csharp run id=continuations
using System;
using System.Threading.Tasks;

// CPS: the result goes to k, and k decides what happens next
static R Fact<R>(int n, Func<long, R> k) => n == 0 ? k(1) : Fact(n - 1, r => k(n * r));

// two continuations: one for success, one for failure
static void Parse(string s, Action<int> ok, Action<string> fail)
{
    if (int.TryParse(s, out int n)) ok(n);
    else fail($"not a number: {s}");
}

Console.WriteLine(Fact(5, r => r));
Console.WriteLine(Fact(5, r => $"5! = {r}"));
foreach (string s in new[] { "42", "4x" })
    Parse(s, n => Console.WriteLine($"ok {n + 1}"), e => Console.WriteLine($"error: {e}"));

// ContinueWith runs a continuation when the task has its result
Task<int> task = Task.Run(() => 6 * 7);
string text = await task.ContinueWith(done => $"the task gave {done.Result}");
Console.WriteLine(text);
```

```output for=continuations
120
5! = 120
ok 43
error: not a number: 4x
the task gave 42
```

- Verdict: idiom. A continuation in C# is a delegate, `Func<T, R>` when it gives a result and `Action<T>` when it does not, and a lambda creates one. `Fact` is generic in the result type, and `Parse` calls exactly one of its two continuations.
- `Task.ContinueWith` attaches a continuation to a task. The compiler turns every `async` method into a state machine whose rest of the method is the continuation of each `await`, so C# programs use CPS through `await` without writing it by hand.
