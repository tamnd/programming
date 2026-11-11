```csharp run id=modifiers
static void Increment(ref int n) => n++;

static bool TryHalf(int n, out int half)
{
    half = n / 2;
    return n % 2 == 0;
}

static double Length(in (double X, double Y) p) => Math.Sqrt(p.X * p.X + p.Y * p.Y);

static int Sum(params int[] values) => values.Sum();

static string Connect(string host, int port = 5432, bool ssl = false) =>
    $"{host}:{port} ssl={ssl}";

int count = 1;
Increment(ref count);
Console.WriteLine(count);

Console.WriteLine(TryHalf(10, out int half) + " " + half);
Console.WriteLine(Length((3, 4)));
Console.WriteLine(Sum(1, 2, 3));
Console.WriteLine(Connect("db", ssl: true));
```

```output for=modifiers
2
True 5
5
6
db:5432 ssl=True
```

- Verdict: native. By default, C# passes arguments by value. The modifiers `ref`, `out`, `in` and `ref readonly` pass a variable by reference. Both the declaration and the call must say `ref` or `out`, so the reader of the call can see that the variable can change.
- An `out` parameter does not need a value before the call, and the method must assign it before it returns.
- C# has optional parameters with default values, named arguments and `params` parameters.

```csharp wrong id=struct-copy
static void Move(Point p) => p.X += 10;

var point = new Point { X = 1 };
Move(point);
Console.WriteLine(point.X);

struct Point
{
    public int X;
}
```

```output for=struct-copy
1
```

`Point` is a struct, so the method gets a copy. The change goes to the copy. Pass it as `ref Point p`, or return the new value.
