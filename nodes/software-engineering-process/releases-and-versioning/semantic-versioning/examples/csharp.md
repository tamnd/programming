```csharp run id=system-version
using System;

var two = Version.Parse("1.2");
var three = Version.Parse("1.2.0");
Console.WriteLine($"{two}: Build={two.Build}, Revision={two.Revision}");
Console.WriteLine($"1.2 < 1.2.0: {two < three}, Equals: {two.Equals(three)}");
Console.WriteLine(Version.Parse("1.2.3.4"));
Console.WriteLine(Version.Parse("01.002.3"));

foreach (var text in new[] { "1.2.3-beta", "1.2.3+build.7" })
{
    try
    {
        Version.Parse(text);
    }
    catch (FormatException error)
    {
        Console.WriteLine($"{text}: {error.GetType().Name}");
    }
}
```

```output for=system-version
1.2: Build=-1, Revision=-1
1.2 < 1.2.0: True, Equals: False
1.2.3.4
1.2.3
1.2.3-beta: FormatException
1.2.3+build.7: FormatException
```

The record below implements SemVer precedence with `IComparable`. The parser uses `[0-9]` and not `\d`, because `\d` in .NET also matches digits outside ASCII.

```csharp run id=semver-record
using System;
using System.Linq;
using System.Numerics;
using System.Text.RegularExpressions;

var ordered = new[] { "1.0.0", "1.0.0-rc.1", "1.0.0-beta.11", "1.0.0-beta.2", "1.0.0-beta",
    "1.0.0-alpha.beta", "1.0.0-alpha.1", "1.0.0-alpha" }
    .Select(SemVersion.Parse)
    .Order()
    .Select(v => v.ToString());
Console.WriteLine(string.Join(" < ", ordered));

var a = SemVersion.Parse("1.4.2+build.7");
var b = SemVersion.Parse("1.4.2+build.8");
Console.WriteLine($"CompareTo: {a.CompareTo(b)}, builds: {a.Build} {b.Build}");

foreach (var text in new[] { "v1.2.3", "1.2", "01.2.3", "١.٢.٣" })
{
    Console.WriteLine($"{text}: {SemVersion.TryParse(text, out _)}");
}
Console.WriteLine($"\\d matches ١: {Regex.IsMatch("١", @"^\d$")}");

sealed record SemVersion(BigInteger Major, BigInteger Minor, BigInteger Patch, string[] Pre, string Build)
    : IComparable<SemVersion>
{
    const string Num = "0|[1-9][0-9]*";
    const string Pid = "0|[1-9][0-9]*|[0-9]*[a-zA-Z-][0-9a-zA-Z-]*";
    static readonly Regex Pattern = new(
        $@"^({Num})\.({Num})\.({Num})(?:-((?:{Pid})(?:\.(?:{Pid}))*))?(?:\+([0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$");

    public static bool TryParse(string text, out SemVersion? version)
    {
        var m = Pattern.Match(text);
        version = m.Success
            ? new SemVersion(BigInteger.Parse(m.Groups[1].Value), BigInteger.Parse(m.Groups[2].Value),
                BigInteger.Parse(m.Groups[3].Value),
                m.Groups[4].Success ? m.Groups[4].Value.Split('.') : [], m.Groups[5].Value)
            : null;
        return m.Success;
    }

    public static SemVersion Parse(string text) =>
        TryParse(text, out var version) ? version! : throw new FormatException(text);

    static bool IsNumeric(string s) => s.All(char.IsAsciiDigit);

    static int CompareIdentifier(string x, string y) => (IsNumeric(x), IsNumeric(y)) switch
    {
        (true, true) => BigInteger.Parse(x).CompareTo(BigInteger.Parse(y)),
        (true, false) => -1,
        (false, true) => 1,
        _ => string.CompareOrdinal(x, y),
    };

    public int CompareTo(SemVersion? other)
    {
        if (other is null) return 1;
        int c = Major.CompareTo(other.Major);
        if (c == 0) c = Minor.CompareTo(other.Minor);
        if (c == 0) c = Patch.CompareTo(other.Patch);
        if (c != 0) return c;
        if (Pre.Length == 0 && other.Pre.Length == 0) return 0;
        if (Pre.Length == 0) return 1;
        if (other.Pre.Length == 0) return -1;
        for (int i = 0; i < Math.Min(Pre.Length, other.Pre.Length); i++)
        {
            c = CompareIdentifier(Pre[i], other.Pre[i]);
            if (c != 0) return Math.Sign(c);
        }
        return Pre.Length.CompareTo(other.Pre.Length);
    }

    public override string ToString() =>
        $"{Major}.{Minor}.{Patch}" + (Pre.Length > 0 ? "-" + string.Join('.', Pre) : "") + (Build != "" ? "+" + Build : "");
}
```

```output for=semver-record
1.0.0-alpha < 1.0.0-alpha.1 < 1.0.0-alpha.beta < 1.0.0-beta < 1.0.0-beta.2 < 1.0.0-beta.11 < 1.0.0-rc.1 < 1.0.0
CompareTo: 0, builds: build.7 build.8
v1.2.3: False
1.2: False
01.2.3: False
١.٢.٣: False
\d matches ١: True
```

- Verdict: library. .NET has no SemVer type in the base class library. `System.Version` represents the version number of an assembly, an operating system or the runtime. It has up to four numbers, no pre-release and no build metadata.
- `Version.Parse("1.2")` gives `Build` and `Revision` the value -1, so 1.2 sorts before 1.2.0, and the two are not equal. `Version.Parse` also accepts leading zeros.
- The NuGet package `NuGet.Versioning` has `NuGetVersion.Parse` and `VersionComparer`. NuGet adds a fourth number and compares pre-release labels without case, so it differs from SemVer 2.0.0 in a few places.
- In PowerShell, the type accelerator `[semver]` is `System.Management.Automation.SemanticVersion`. That type is part of the PowerShell SDK, not of .NET.
- The record `SemVersion` compares by precedence in `CompareTo`. Its generated `Equals` compares the array `Pre` by reference, so do not use `Equals` or `==` on it for precedence.
