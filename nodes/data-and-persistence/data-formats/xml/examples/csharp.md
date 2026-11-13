```csharp run id=parse-and-write
using System.Xml;
using System.Xml.Linq;

string text = "<library city=\"Zürich\"><book id=\"b1\"><title>Tom &amp; Jerry</title><note><![CDATA[a < b]]></note></book></library>";
XElement root = XElement.Parse(text);
XElement book = root.Element("book")!;
Console.WriteLine($"{root.Name} {(string?)root.Attribute("city")}");
Console.WriteLine($"{(string?)book.Attribute("id")} {(string?)book.Element("title")} {(string?)book.Element("note")}");

var draft = new XElement("book", new XAttribute("id", "b2"), new XElement("title", "Q&A <draft>"));
Console.WriteLine(draft.ToString(SaveOptions.DisableFormatting));

try
{
    XElement.Parse("<a><b></a>");
}
catch (XmlException e)
{
    Console.WriteLine($"XmlException at line {e.LineNumber}, position {e.LinePosition}");
}
```

```output for=parse-and-write
library Zürich
b1 Tom & Jerry a < b
<book id="b2"><title>Q&amp;A &lt;draft&gt;</title></book>
XmlException at line 1, position 9
```

- Verdict: stdlib. LINQ to XML in `System.Xml.Linq` parses a text with `XElement.Parse` and writes it with `ToString`. A cast of an element or attribute to `string` gives its text, with the entity references and the CDATA section resolved. The text of a new element is escaped on output. A text that is not well-formed raises `XmlException` with the line and the position.
