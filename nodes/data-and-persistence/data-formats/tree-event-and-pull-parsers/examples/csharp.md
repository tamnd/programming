```csharp run id=three-models
using System.Globalization;
using System.Xml;
using System.Xml.Linq;

string text = """
    <orders>
      <order id="1"><total>9.50</total></order>
      <order id="2"><total>20.00</total></order>
      <order id="3"><total>4.25</total></order>
    </orders>
    """;

CultureInfo.CurrentCulture = CultureInfo.InvariantCulture;

// Tree: LINQ to XML loads the whole document.
XDocument doc = XDocument.Parse(text);
decimal tree = doc.Descendants("total").Sum(t => (decimal)t);
Console.WriteLine($"tree: {tree}");

// Pull: XmlReader moves forward one node at a time.
decimal pulled = 0;
using (XmlReader reader = XmlReader.Create(new StringReader(text)))
{
    while (reader.Read())
    {
        if (reader.NodeType == XmlNodeType.Element && reader.Name == "total")
        {
            pulled += decimal.Parse(reader.ReadElementContentAsString(), CultureInfo.InvariantCulture);
        }
    }
}
Console.WriteLine($"pull: {pulled}");

// Mixed: pull to each order, then load only that element as a tree.
using (XmlReader reader = XmlReader.Create(new StringReader(text)))
{
    while (!reader.EOF)
    {
        if (reader.NodeType == XmlNodeType.Element && reader.Name == "order")
        {
            var order = (XElement)XNode.ReadFrom(reader);
            Console.WriteLine($"order {(string?)order.Attribute("id")}: {(decimal)order.Element("total")!}");
        }
        else
        {
            reader.Read();
        }
    }
}
```

```output for=three-models
tree: 33.75
pull: 33.75
order 1: 9.50
order 2: 20.00
order 3: 4.25
```

- Verdict: stdlib. `XDocument.Parse` builds the whole tree. `XmlReader` is the pull parser of .NET: each call to `Read` moves to the next node, and the program reads the node where the reader stands. `XNode.ReadFrom` reads the element at the current position of the reader as a small tree and moves the reader past it. This mix gives the convenience of a tree for each record and the memory use of a stream for the file.
