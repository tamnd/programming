```csharp run id=light
foreach (var light in Enum.GetValues<Light>())
{
    Console.WriteLine($"{Lower(light)} {(int)light} then {Lower(After(light))}");
}
Console.WriteLine($"no name: {(Light)7}");

static Light After(Light light) => light switch
{
    Light.Red => Light.Green,
    Light.Green => Light.Amber,
    Light.Amber => Light.Red,
    _ => throw new ArgumentOutOfRangeException(nameof(light)),
};

static string Lower(Light light) => light.ToString().ToLowerInvariant();

enum Light { Red, Amber, Green }
```

```output for=light
red 0 then green
amber 1 then red
green 2 then amber
no name: 7
```

- Verdict: native. An `enum` is a value type over an integral type, `int` by default.
- `Enum.GetValues<Light>()` lists the members. A cast converts between the enum and its number.
- A cast can make a value with no name, such as `(Light)7`, so the `switch` needs a `_` branch.
