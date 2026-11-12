```csharp run id=extensions
using System;
using System.Collections.Generic;

class Document
{
    public readonly string Text;
    private readonly Dictionary<Type, object> extensions = new();

    public Document(string text) { Text = text; }

    public void AddExtension(object extension) => extensions[extension.GetType()] = extension;

    public T? GetExtension<T>() where T : class =>
        extensions.TryGetValue(typeof(T), out var e) ? (T)e : null;
}

class WordCount
{
    private readonly Document doc;
    public WordCount(Document doc) { this.doc = doc; }
    public int Count() => doc.Text.Split(' ', StringSplitOptions.RemoveEmptyEntries).Length;
}

class Upper
{
    private readonly Document doc;
    public Upper(Document doc) { this.doc = doc; }
    public string Text() => doc.Text.ToUpperInvariant();
}

class Program
{
    static void ShowUpper(Document doc)
    {
        var upper = doc.GetExtension<Upper>();
        Console.WriteLine(upper is null ? "upper: not supported" : $"upper: {upper.Text()}");
    }

    static void Main()
    {
        var doc = new Document("the quick brown fox");
        doc.AddExtension(new WordCount(doc));
        Console.WriteLine($"words: {doc.GetExtension<WordCount>()!.Count()}");
        ShowUpper(doc);
        doc.AddExtension(new Upper(doc));
        ShowUpper(doc);
    }
}
```

```output
words: 4
upper: not supported
upper: THE QUICK BROWN FOX
```

- Verdict: idiom. A `Dictionary<Type, object>` holds the extensions, and a generic method finds one by its type. WCF has the same form in `IExtensibleObject<T>` and `IExtensionCollection<T>`.
