```csharp run id=exhaustive-matching-csharp
using System;

static string Action(Light light) => light switch
{
    Light.Red => "stop",
    Light.Yellow => "slow down",
    Light.Green => "go",
    _ => throw new ArgumentOutOfRangeException(nameof(light)),
};

foreach (Light light in Enum.GetValues<Light>())
{
    Console.WriteLine($"{light.ToString().ToLowerInvariant()}: {Action(light)}");
}

enum Light { Red, Yellow, Green }
```

```output for=exhaustive-matching-csharp
red: stop
yellow: slow down
green: go
```

- Verdict: native. The compiler gives a warning, not an error, for a switch expression that does not cover each input, and the runtime throws an exception when no arm matches. A C# enum can hold an integer that is not a named member, such as `(Light)10`, so the switch also needs the discard arm `_` to be exhaustive.
