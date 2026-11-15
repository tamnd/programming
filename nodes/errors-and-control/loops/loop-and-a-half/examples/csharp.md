```csharp run id=tryparse-retry-and-pattern-in-the-condition
var inputs = new Queue<string>(new[] { "ten", "", "42" });
int value;
while (true)
{
    string text = inputs.Dequeue();
    if (int.TryParse(text, out value)) break;
    Console.WriteLine($"not a number: \"{text}\"");
}
Console.WriteLine($"value: {value}");

// The pattern { } matches any non-null value and names it.
var reader = new StringReader("alpha\nbeta\n");
while (reader.ReadLine() is { } line)
    Console.WriteLine($"line: {line}");
```

```output for=tryparse-retry-and-pattern-in-the-condition
not a number: "ten"
not a number: ""
value: 42
line: alpha
line: beta
```

- Verdict: idiom. The retry loop reads, tests with `TryParse` and complains only when the test fails, so it reads 3 times and complains 2 times. The compiler knows that `value` is assigned after the loop, because the only exit is the `break` after `TryParse`. In the second loop, `is { } line` reads, tests for `null` and names the line in one condition.
