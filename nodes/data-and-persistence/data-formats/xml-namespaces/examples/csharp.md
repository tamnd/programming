```csharp run id=expanded-names
using System.Xml.Linq;

string text = """
    <catalog xmlns="urn:example:books" xmlns:m="urn:example:meta">
      <book id="b1" m:lang="en">
        <title>Tide tables</title>
        <price xmlns="urn:example:money" currency="EUR">32.18</price>
      </book>
    </catalog>
    """;
XNamespace b = "urn:example:books";
XNamespace m = "urn:example:meta";
XNamespace money = "urn:example:money";

XElement root = XElement.Parse(text);
XElement book = root.Element(b + "book")!;
Console.WriteLine(root.Name);
Console.WriteLine($"{(string?)book.Attribute(m + "lang")} {(string?)book.Element(b + "title")} {(string?)book.Element(money + "price")}");
Console.WriteLine(root.Element("book") is null);
Console.WriteLine(root.GetPrefixOfNamespace(m));

var item = new XElement(b + "catalog",
    new XAttribute(XNamespace.Xmlns + "m", m),
    new XElement(b + "book", new XAttribute(m + "lang", "fr"), new XElement(b + "title", "Moon phases")));
Console.WriteLine(item.ToString(SaveOptions.DisableFormatting));
```

```output for=expanded-names
{urn:example:books}catalog
en Tide tables 32.18
True
m
<catalog xmlns:m="urn:example:meta" xmlns="urn:example:books"><book m:lang="fr"><title>Moon phases</title></book></catalog>
```

- Verdict: stdlib. LINQ to XML gives each name as an `XName`, made from an `XNamespace` and a local name with the operator `+`. The cast of an `XElement` or `XAttribute` to `string` gives its text. `Element("book")` with no namespace finds nothing, because the element is in the books namespace. To choose the prefix of a namespace in the output, the program adds an `xmlns:m` attribute with `XNamespace.Xmlns`.
