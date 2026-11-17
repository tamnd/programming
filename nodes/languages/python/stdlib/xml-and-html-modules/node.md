---
tag: 9H96PB
kind: feature
owner: python
slug: xml-and-html-modules
name: XML and HTML modules
aliases: [xml.etree.ElementTree, ElementTree, xml.dom.minidom, minidom, html.parser, HTMLParser, html.escape, XML parsing, XML security]
area: languages/python/stdlib
level: advanced
summary: >-
  xml.etree.ElementTree parses and builds XML as a tree of elements, and xml.dom.minidom offers a small DOM. The html package escapes text and has a simple HTML parser. Untrusted XML needs care.
grammar: null
spec: {standard: null, url: "https://docs.python.org/3.15/library/markup.html", locator: "Structured Markup Processing Tools"}
proposals: []
see_also: [python/file-format-modules, python/pickle-and-object-serialization, python/data-compression]
sources:
  - url: https://docs.python.org/3.15/library/markup.html
    title: Structured Markup Processing Tools, Python 3.15 documentation
    locator: Structured Markup Processing Tools
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/xml.etree.elementtree.html
    title: xml.etree.ElementTree, The ElementTree XML API, Python 3.15 documentation
    locator: Tutorial; Parsing XML with Namespaces; XPath support; indent(); Element objects
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/xml.dom.minidom.html
    title: xml.dom.minidom, Minimal DOM implementation, Python 3.15 documentation
    locator: Introduction
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/html.html
    title: html, HyperText Markup Language support, Python 3.15 documentation
    locator: escape(); unescape()
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/html.parser.html
    title: html.parser, Simple HTML and XHTML parser, Python 3.15 documentation
    locator: Introduction; HTMLParser methods
    accessed: 2026-11-16
  - url: https://docs.python.org/3.15/library/xml.html
    title: XML Processing Modules, Python 3.15 documentation
    locator: Introduction; XML security
    accessed: 2026-11-16
---

## Synopsis

```python run id=parse
import xml.etree.ElementTree as ET

doc = """<library>
  <book id="b1" year="1978"><title>The C Programming Language</title></book>
  <book id="b2" year="1985"><title>Structure and Interpretation of Computer Programs</title></book>
</library>"""
root = ET.fromstring(doc)
for book in root.findall("book"):
    print(book.get("id"), book.get("year"), book.find("title").text)
print(root.find("book[@year='1985']/title").text)
```

```output for=parse
b1 1978 The C Programming Language
b2 1985 Structure and Interpretation of Computer Programs
Structure and Interpretation of Computer Programs
```

## Semantics

**r1.** "Python supports a variety of modules to work with various forms of structured data markup."[^1] For XML, the main one is ElementTree, which "implements a simple and efficient API for parsing and creating XML data."[^2] Example: [`parse`](#parse).

**r2.** `fromstring` returns the root `Element`. "Element.findall() finds only elements with a tag which are direct children of the current element. Element.find() finds the first child with a particular tag, and Element.text accesses the element’s text content. Element.get() accesses the element’s attributes."[^2] Example: [`parse`](#parse).

**r3.** The find methods take a path. The module "provides limited support for XPath expressions for locating elements in a tree."[^2] A path such as `book[@year='1985']/title` filters by an attribute.

**r4.** An element has `text` and `tail`. The text is "the text between the element’s start tag and its first child or end tag", and the tail is "the text between the element’s end tag and the next tag".[^2] Example: [`mixed-content`](#mixed-content).

**r5.** In a document with namespaces, tags "get expanded to {uri}sometag where the prefix is replaced by the full URI. Also, if there is a default namespace, that full URI gets prepended to all of the non-prefixed tags."[^2] Example: [`namespaces`](#namespaces).

**r6.** `xml.dom.minidom` "is a minimal implementation of the Document Object Model interface, with an API similar to that in other languages."[^3] Example: [`minidom`](#minidom).

**r7.** `html.escape` converts "the characters &, < and > in string s to HTML-safe sequences."[^4] By default it converts the quote characters too. `html.unescape` does the reverse. Example: [`html-tools`](#html-tools).

**r8.** `html.parser.HTMLParser` is a base class: you subclass it and override the handler methods, such as `handle_starttag` and `handle_data`. For a start tag, "The tag argument is the name of the tag converted to lower case."[^5] Example: [`html-tools`](#html-tools).

**r9.** "An attacker can abuse XML features to carry out denial of service attacks, access local files, generate network connections to other machines, or circumvent firewalls when attacker-controlled XML is being parsed, in Python or elsewhere."[^6] The XML security section lists the attacks for each parser.

## Examples

```python run id=build
import xml.etree.ElementTree as ET

root = ET.Element("config", version="2")
server = ET.SubElement(root, "server")
server.text = "a < b & c"
ET.SubElement(root, "debug", enabled="false")
print(ET.tostring(root, encoding="unicode"))
ET.indent(root)
print(ET.tostring(root, encoding="unicode"))
```

```output for=build
<config version="2"><server>a &lt; b &amp; c</server><debug enabled="false" /></config>
<config version="2">
  <server>a &lt; b &amp; c</server>
  <debug enabled="false" />
</config>
```

`SubElement` makes a child and adds it to the parent. `tostring` escapes `<` and `&` in the text. `indent` "Appends whitespace to the subtree to indent the tree visually."[^2]

```python run id=namespaces
import xml.etree.ElementTree as ET

doc = """<feed xmlns="http://www.w3.org/2005/Atom">
  <entry><title>First</title></entry>
  <entry><title>Second</title></entry>
</feed>"""
root = ET.fromstring(doc)
print(root.tag)
print(root.findall("entry"))
ns = {"atom": "http://www.w3.org/2005/Atom"}
print([e.find("atom:title", ns).text for e in root.findall("atom:entry", ns)])
```

```output for=namespaces
{http://www.w3.org/2005/Atom}feed
[]
['First', 'Second']
```

The default namespace is part of every tag name, so `findall("entry")` finds nothing (rule r5). Pass a dict of prefixes, and use the prefix in the path.

```python run id=minidom
from xml.dom import minidom

dom = minidom.parseString("<menu><item price='3'>tea</item><item price='5'>cake</item></menu>")
for item in dom.getElementsByTagName("item"):
    print(item.getAttribute("price"), item.firstChild.data)
print(dom.documentElement.toprettyxml(indent="  "), end="")
```

```output for=minidom
3 tea
5 cake
<menu>
  <item price="3">tea</item>
  <item price="5">cake</item>
</menu>
```

The DOM methods have the names of the W3C DOM, such as `getElementsByTagName` (rule r6). In the DOM, the text is a child node, so `firstChild.data` reads it.

```python run id=html-tools
import html
from html.parser import HTMLParser

text = '<script>alert("x")</script> & more'
safe = html.escape(text)
print(safe)
print(html.unescape(safe) == text, html.unescape("&eacute;&#233;&gt;"))

class LinkParser(HTMLParser):
    def __init__(self):
        super().__init__()
        self.links = []

    def handle_starttag(self, tag, attrs):
        if tag == "a":
            self.links.append(dict(attrs).get("href"))

parser = LinkParser()
parser.feed('<p>See <a href="/docs">docs</a> and <A HREF="https://example.com">this<p>')
print(parser.links)
```

```output for=html-tools
&lt;script&gt;alert(&quot;x&quot;)&lt;/script&gt; &amp; more
True éé>
['/docs', 'https://example.com']
```

`escape` makes text safe to put in an HTML page (rule r7). The parser gives tag and attribute names in lower case, so `<A HREF=...>` matches `'a'` (rule r8). It accepts HTML that is not well formed: "This parser does not check that end tags match start tags".[^5]

## Versions

> [!since v="3.9"]
> `xml.etree.ElementTree.indent` is new.[^2]

## Pitfalls

```python wrong id=mixed-content
import xml.etree.ElementTree as ET

p = ET.fromstring("<p>Hello <b>big</b> world</p>")
print(repr(p.text), repr(p.find("b").text), repr(p.find("b").tail))
print("".join(p.itertext()))
```

```output for=mixed-content
'Hello ' 'big' ' world'
Hello big world
```

`p.text` is only `'Hello '`. The text after `<b>` is the tail of `b`, not part of `p.text` (rule r4). Use `itertext` to get all the text of an element.

```python fails id=bad-xml expect=exception:ParseError
import xml.etree.ElementTree as ET

ET.fromstring("<a><b>text</a></b>")
```

XML must be well formed, so the parser stops at the first wrong end tag. The HTML parser is different and accepts such input (rule r8).

Do not test an element with `if element:`. "Caution: Elements with no subelements will test as False."[^2] Use `element is not None` to check the result of `find`, and `len(element)` to count the children.

## Comparison

ElementTree is the usual choice in Python. The minidom docs say "Users who are not already proficient with the DOM should consider using the xml.etree.ElementTree module for their XML processing instead."[^3] Code that already uses the W3C DOM API in another language maps more directly to minidom. For data files that are not markup, see [[python/file-format-modules]]. Before you parse XML from an untrusted source, read the XML security section of the docs (rule r9).
