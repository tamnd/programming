```csharp run id=xml-doc-comments
using System.Reflection;

Console.WriteLine(Geometry.Area(2));

// Comments are gone after compilation; reflection sees attributes, not comments.
var m = typeof(Geometry).GetMethod("Area")!;
Console.WriteLine(m.GetCustomAttribute<System.ComponentModel.DescriptionAttribute>()?.Description);

// Text that looks like a comment inside a string is part of the string.
Console.WriteLine("// not a comment".Length);

// #if false removes a region that may itself contain block comments, which /* */ cannot do.
#if false
/* first */ Console.WriteLine("removed"); /* second */
#endif
Console.WriteLine("done");

static class Geometry
{
    /// <summary>Returns the area of a circle.</summary>
    /// <param name="r">The radius.</param>
    [System.ComponentModel.Description("area of a circle")]
    public static double Area(double r) => Math.PI * r * r; // the compiler ignores this
}
```

```output for=xml-doc-comments
12.566370614359172
area of a circle
16
done
```

- Verdict: native. C# has `//` and `/* */`, and a `///` comment holds XML that the compiler writes to a documentation file when `GenerateDocumentationFile` is set. Reflection cannot read comments, so a program that needs the text at run time uses an attribute such as `Description`. `#if false` removes a region that holds block comments.
