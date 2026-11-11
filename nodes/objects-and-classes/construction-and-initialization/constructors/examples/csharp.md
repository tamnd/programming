```csharp run id=constructors-and-initializers
var r = new Range(3, 7);
Console.WriteLine(r.Size);
Console.WriteLine(Range.Parse("10..12").Size);
try
{
    _ = new Range(9, 1);
}
catch (ArgumentException e)
{
    Console.WriteLine(e.Message);
}

var p = new Person { Name = "Ana" };  // an object initializer sets init and required members
Console.WriteLine($"{p.Name} {p.Email ?? "(no email)"}");
Console.WriteLine(new Label().Text);

class Range
{
    public int Lo { get; }
    public int Hi { get; }

    public Range(int lo, int hi)
    {
        if (lo > hi) throw new ArgumentException($"empty range {lo}..{hi}");
        (Lo, Hi) = (lo, hi);
    }

    public Range(int hi) : this(0, hi) { }  // delegates to another constructor

    public static Range Parse(string text)  // a static factory
    {
        var parts = text.Split("..");
        return new Range(int.Parse(parts[0]), int.Parse(parts[1]));
    }

    public int Size => Hi - Lo + 1;
}

class Person
{
    public required string Name { get; init; }  // must be set when the object is created
    public string? Email { get; init; }
}

class Widget
{
    public string Text;

    protected Widget() { Text = Render(); }

    protected virtual string Render() => "widget";
}

class Label : Widget
{
    private readonly string text = "hello";  // C# runs field initializers before the base constructor

    protected override string Render() => "label: " + text;
}
```

```output for=constructors-and-initializers
5
3
empty range 9..1
Ana (no email)
label: hello
```

```csharp fails expect=compile id=required-member
var p = new Person();  // Name is required but not set
Console.WriteLine(p.Name);

class Person
{
    public required string Name { get; init; }
}
```

- Verdict: native. A constructor can check its arguments and delegate to another with `: this(...)` or call a base constructor with `: base(...)`. Object initializers set properties after the constructor runs, and `init` accessors allow this only during creation. A `required` member must be set by every object initializer, which the compiler checks. C# runs the field initializers of a class before it calls the base constructor, so the virtual call from `Widget`'s constructor sees `text` already set, unlike in Java and JavaScript. Fields set in the constructor body are still unset at that point.
- The failing block creates a `Person` without setting the required `Name`, and the compiler rejects it with error CS9035.
