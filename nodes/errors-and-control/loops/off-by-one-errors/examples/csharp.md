```csharp run id=ranges-hat-and-index-out-of-range
int[] a = { 10, 20, 30, 40, 50 };

// The range 1..3 takes the indexes 1 and 2.
Console.WriteLine(string.Join(",", a[1..3]));

// ^1 is the last element; ^0 is one past the end.
Console.WriteLine($"{a[^1]} {a[a.Length - 1]}");
Console.WriteLine(string.Join(",", a[2..^0]));

// The length of a range is end - start.
Range r = 1..4;
var (offset, length) = r.GetOffsetAndLength(a.Length);
Console.WriteLine($"offset {offset}, length {length}");

try
{
    Console.WriteLine(a[a.Length]);
}
catch (IndexOutOfRangeException)
{
    Console.WriteLine("index 5 is out of range");
}

// for with <= on a one-based count: 1 to 5 is five passes.
int passes = 0;
for (int i = 1; i <= 5; i++) passes++;
Console.WriteLine($"passes: {passes}");
```

```output for=ranges-hat-and-index-out-of-range
20,30
50 50
30,40,50
offset 1, length 3
index 5 is out of range
passes: 5
```

- Verdict: native. A C# range `start..end` excludes `end`, so `a[1..3]` has two elements and `GetOffsetAndLength` gives the length 3 for `1..4`. The index `^1` counts from the end and is the last element, so the code does not write `a.Length - 1`. The array access `a[5]` throws an `IndexOutOfRangeException`. A loop from 1 with `<=` is also correct, but the bounds and the test must match.
