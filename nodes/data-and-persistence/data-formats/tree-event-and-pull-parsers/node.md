---
tag: 26BBYN
kind: concept
slug: tree-event-and-pull-parsers
name: Tree, event and pull parsers
aliases: [DOM, SAX, StAX, push parser, streaming parser]
area: data-and-persistence/data-formats
level: core
summary: >-
  A tree parser loads a whole document into memory. An event parser calls the program for each part, and a pull parser gives the next part when the program asks.
see_also: [xml, xml-namespaces, stream-parse-a-large-json-file, json]
sources:
- url: http://www.saxproject.org/event.html
  title: SAX project, Events vs. Trees
  locator: the whole page
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.xml/javax/xml/stream/XMLStreamReader.html
  title: Java SE 25 API, javax.xml.stream.XMLStreamReader
  locator: the interface description
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.xml/org/xml/sax/ContentHandler.html
  title: Java SE 25 API, org.xml.sax.ContentHandler
  locator: characters
- url: https://learn.microsoft.com/en-us/dotnet/api/system.xml.xmlreader
  title: .NET API documentation, XmlReader Class
  locator: the summary and the remarks
- url: https://docs.python.org/3.14/library/xml.etree.elementtree.html
  title: The Python Standard Library, xml.etree.ElementTree, Python 3.14
  locator: Pull API for non-blocking parsing; iterparse; XMLPullParser Objects
- url: https://pkg.go.dev/encoding/xml
  title: Go standard library, package encoding/xml
  locator: Decoder.Token; Decoder.DecodeElement; Unmarshal
- url: https://docs.python.org/3.14/library/xml.sax.html
  title: The Python Standard Library, xml.sax, Python 3.14
  locator: the module description
- url: http://www.saxproject.org/sax1-history.html
  title: SAX project, SAX 1.0, Genesis
  locator: the whole page
- url: http://www.saxproject.org/
  title: SAX project, About SAX
  locator: the whole page
---

## Idea

A parser reads the text of a document and gives its structure to the program. There are three ways to give it. A tree parser reads the whole document and returns a tree of nodes. An event parser, also called a push parser, calls a function of the program for each part of the document as it reads. A pull parser gives the parts one at a time, when the program asks for the next one. The three models are most visible in XML, where each one has a standard API: DOM for trees, SAX for events and StAX for pull parsing in Java. The same models exist for JSON and for other formats.

The SAX project describes the first two. A tree-based API maps "an XML document into an internal tree structure, then allow an application to navigate that tree." An event-based API "reports parsing events (such as the start and end of elements) directly to the application through callbacks, and does not usually build an internal tree."[^1] The pull model turns the event model around. "The XMLStreamReader is designed to iterate over XML using next() and hasNext()."[^2]

The choice is a trade between convenience and memory. A tree is easy to use: the program can go to any node, at any time, in any order. But the tree must fit in memory, and it has an object for each node, so it takes more memory than the text. A stream of events or tokens needs only a small, fixed amount of memory, but the program sees each part only once, in document order.

## How it works

The document below has three orders. Each model below adds the totals.

```xml
<orders>
  <order id="1"><total>9.50</total></order>
  <order id="2"><total>20.00</total></order>
  <order id="3"><total>4.25</total></order>
</orders>
```

**Tree.** The parser reads all of the text and builds a node for each element, attribute and piece of text. The program then walks the tree or searches it, for example with XPath. In Python, `ElementTree.fromstring` returns the root element. In Java, `DocumentBuilder.parse` returns a DOM `Document`. The program can change the tree and write it back.

**Event (push).** The program gives the parser a handler object. The parser reads the text and calls the methods of the handler: one call for each start tag, each piece of text and each end tag. The parser controls the loop, and the program sees one event at a time. To know where it is, the handler must keep its own state, such as the name of the current element or a stack of the open elements. The trace below shows the events of SAX for a small document.

```python run id=sax-events
import xml.sax


class Trace(xml.sax.ContentHandler):
    def startElement(self, name, attrs):
        print("start", name, dict(attrs))

    def characters(self, content):
        print("text ", repr(content))

    def endElement(self, name):
        print("end  ", name)


xml.sax.parseString(b'<order id="1">Tea &amp; cake<total>9.50</total></order>', Trace())
```

```output for=sax-events
start order {'id': '1'}
text  'Tea '
text  '&'
text  ' cake'
start total {}
text  '9.50'
end   total
end   order
```

The text `Tea &amp; cake` comes in three pieces. This is permitted: "SAX parsers may return all contiguous character data in a single chunk, or they may split it into several chunks".[^3] A handler must join the pieces before it uses the text.

**Pull.** The program owns the loop. It asks the parser for the next event or token, looks at it and decides what to do. The state can be in the local variables and the call stack of the program, as in a recursive descent parser. The program can also stop early, skip a subtree or give the reader to a function that reads one record. The Java documentation says that StAX "is designed to be the lowest level and most efficient way to read XML data."[^2] The .NET documentation says: "XmlReader uses a pull model to retrieve data."[^4]

A pull parser can also take its input in pieces, from a network socket for example. The Python `XMLPullParser` "does not require a blocking read to obtain the XML data, and is instead fed with data incrementally with XMLPullParser.feed() calls."[^5]

```python run id=feed-in-pieces
import xml.etree.ElementTree as ET

parser = ET.XMLPullParser(events=("start", "end"))
for chunk in ['<orders><order id="1">', '<total>9.50</to', 'tal></order></orders>']:
    parser.feed(chunk)
    for event, element in parser.read_events():
        print(repr(chunk), event, element.tag)
parser.close()
```

```output for=feed-in-pieces
'<orders><order id="1">' start orders
'<orders><order id="1">' start order
'<total>9.50</to' start total
'tal></order></orders>' end total
'tal></order></orders>' end order
'tal></order></orders>' end orders
```

The end tag `</total>` is split between two pieces, and the parser reports the end of `total` only when it has the whole tag.

**Mixed.** Many programs use a pull parser to find each record and a tree for the record itself. The program moves to the start of each `order`, reads only that element as a small tree, uses it and drops it. In Go, `Decoder.Token` moves forward and `DecodeElement` reads one element into a struct.[^6] In .NET, `XNode.ReadFrom` reads the element at the position of an `XmlReader`. In Python, `iterparse` builds the elements and the program clears each one after use. The example files show this pattern.

## In languages

| Language | Tree | Event (push) | Pull |
|---|---|---|---|
| Python | `xml.etree.ElementTree`, `xml.dom.minidom` | `xml.sax`[^7] | `iterparse`, `XMLPullParser`[^5] |
| Java | DOM, `DocumentBuilder` | SAX, `SAXParser` | StAX, `XMLStreamReader`[^2] |
| C# | `XDocument`, `XmlDocument` | none in the base library | `XmlReader`[^4] |
| Go | `xml.Unmarshal` into structs | none | `Decoder.Token`[^6] |
| JavaScript | `DOMParser` in browsers | packages such as sax and saxes | none built in |
| C | libxml2 tree | Expat, libxml2 SAX | libxml2 `xmlTextReader` |

For JSON, the same models exist. `json.loads` and `JSON.parse` build a tree of values. Streaming JSON parsers give events or tokens, such as the Jackson `JsonParser` in Java and `Utf8JsonReader` in .NET (see Stream-parse a large JSON file).

## Variations and related ideas

- **Pull parsers for other data.** A tokenizer of a programming language is a pull parser: the syntax analyzer asks for the next token. An iterator over the lines of a file is the same idea at a smaller scale.
- **Filters and pipelines.** An event handler can pass the events on to another handler, after it changes some of them. SAX calls such a handler a filter. A program can chain filters, and the document never needs to be in memory.
- **Lazy trees.** Some libraries build a tree only for the parts that the program reads, or map the nodes to offsets in the text. This is between the tree and the stream models.
- **Writers.** The same split applies to output. A program can build a tree and serialise it, or call a stream writer, such as `XMLStreamWriter` in Java or `XmlWriter` in .NET, for each element. A stream writer can write a file of any size.

## Pitfalls

- **A tree for a large file.** The tree of a very large file does not fit in memory. The SAX project gives the example: "If your XML document were 20MB large (or even just 2MB), it would be very inefficient to construct and traverse an in-memory parse tree just to locate this one piece of contextual information".[^1] Use a stream for large or unbounded input.
- **Text in pieces.** An event or pull parser can split the text of one element into several events.[^3] A program that uses only the first piece reads `Tea ` instead of `Tea & cake`. Collect the text until the end tag, or use a method that reads the whole text of an element, such as `getElementText` in StAX.
- **`iterparse` keeps the tree.** Python `iterparse` builds the tree as it reads, but "The tree is only built incrementally, it is not freed incrementally".[^5] Without a call to `clear` on each processed element, it uses as much memory as a tree parser.
- **State in a handler.** An event handler that keeps the state in flags breaks when the document has nested elements with the same name, or when an element appears in an unexpected place. Keep a stack of the open elements, or use a pull parser.
- **Errors after the work.** A stream parser finds an error in the middle or at the end of the document, after the program has already used the first records. A tree parser fails before the program sees any data. When a partial result is a problem, validate the input first, or write the results in a transaction.
- **Whitespace events.** The line feeds and spaces between elements are text. A stream program that expects the next event to be a start tag gets a text event instead. Skip whitespace-only text, as the method `nextTag` of StAX does.

## Cost

Let n be the size of the text. All three models read each byte once, so the time is O(n). A tree uses O(n) memory, with a large constant, because each node is an object with pointers to its parent, its children and its siblings. An event or pull parser uses memory in proportion to the depth of nesting and the size of the largest token, not the size of the document. The mixed pattern uses memory in proportion to the largest record.

## History

The XML-DEV mailing list started the design of SAX on 13 December 1997. Peter Murray-Rust, the author of the XML browser JUMBO, wanted one event-based API for all Java XML parsers, and David Megginson coordinated the discussion and wrote the proposal.[^8] The first draft came out on 12 January 1998, and SAX 1.0 "was released on Monday 11 May 1998, less than five months after SAX was first proposed."[^8] SAX was "the first widely adopted API for XML in Java, and is a “de facto” standard", and the current version is SAX 2.0.1.[^9] The Java platform added StAX, the pull API of the package `javax.xml.stream`.[^2]

## References
