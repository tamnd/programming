```csharp run id=change
var numbers = new List<int> { 1, 2, 3 };
Rebind(numbers);
Console.WriteLine($"after Rebind: {string.Join(", ", numbers)}");
Append(numbers);
Console.WriteLine($"after Append: {string.Join(", ", numbers)}");
RebindRef(ref numbers);
Console.WriteLine($"after RebindRef: {string.Join(", ", numbers)}");

var alias = numbers;
alias[0] = 100;
Console.WriteLine($"numbers[0]: {numbers[0]}");
Console.WriteLine($"same object: {ReferenceEquals(alias, numbers)}");

var point = new Point { X = 1 };
var pointCopy = point;
pointCopy.X = 100;
Console.WriteLine($"point.X: {point.X}");

static void Rebind(List<int> items) => items = new List<int> { 0, 0, 0 };
static void Append(List<int> items) => items.Add(4);
static void RebindRef(ref List<int> items) => items = new List<int> { 0, 0, 0 };

struct Point
{
    public int X;
}
```

```output for=change
after Rebind: 1, 2, 3
after Append: 1, 2, 3, 4
after RebindRef: 0, 0, 0
numbers[0]: 100
same object: True
point.X: 1
```

- Verdict: native. A variable of a class type holds a reference. A variable of a struct type holds the value, so `pointCopy` is a separate copy.
- By default, a parameter receives a copy of the argument. For a class, that copy is the reference, so `Rebind` changes only its parameter.
- A `ref` parameter is an alias for the variable of the caller. Both the method and the call must say `ref`, and `RebindRef` replaces the list of the caller.
- `ReferenceEquals` compares identity. C# also has pointer types, but only in code marked `unsafe` and compiled with unsafe blocks allowed.
