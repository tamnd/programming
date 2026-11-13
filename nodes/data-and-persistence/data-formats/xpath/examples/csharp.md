```csharp run id=select-and-evaluate
using System.Globalization;
using System.Xml.Linq;
using System.Xml.XPath;

CultureInfo.CurrentCulture = CultureInfo.InvariantCulture;

string text = """
    <library>
      <book id="b1" year="1999"><title>Alpha</title><price>12.50</price></book>
      <book id="b2" year="2008"><title>Beta</title><price>30</price></book>
      <book id="b3" year="2017"><title>Gamma</title><price>8.25</price></book>
    </library>
    """;
XDocument doc = XDocument.Parse(text);

foreach (XElement title in doc.XPathSelectElements("/library/book[@year > 2000]/title"))
{
    Console.WriteLine($"title: {title.Value}");
}
Console.WriteLine($"count: {doc.XPathEvaluate("count(//book)")}");
Console.WriteLine($"sum: {doc.XPathEvaluate("sum(//price)")}");
Console.WriteLine($"cheap: {doc.XPathEvaluate("string(//book[price < 10]/@id)")}");
Console.WriteLine($"is double: {doc.XPathEvaluate("count(//book)") is double}");

try
{
    doc.XPathSelectElements("//book[@year >]");
}
catch (XPathException e)
{
    Console.WriteLine($"XPathException: {e.Message}");
}
```

```output for=select-and-evaluate
title: Beta
title: Gamma
count: 3
sum: 50.75
cheap: b3
is double: True
XPathException: Expression must evaluate to a node-set.
```

- Verdict: stdlib. The namespace `System.Xml.XPath` adds the extension methods `XPathSelectElements` and `XPathEvaluate` to LINQ to XML. They implement XPath 1.0. `XPathEvaluate` returns an `object`: a `double` for a number, a `string`, a `bool`, or a sequence of nodes. An expression with a syntax error raises `XPathException`.
