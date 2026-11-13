---
tag: BDGAFG
kind: format
slug: xml
name: XML
aliases: [Extensible Markup Language]
area: data-and-persistence/data-formats
level: core
summary: >-
  XML is a text format for trees of named elements with attributes and text. A document is well-formed when its tags nest correctly and it has exactly one root element.
see_also: [json, csv, xml-namespaces, tree-event-and-pull-parsers, xpath, data-interchange-format, deserialization-of-untrusted-data]
sources:
- url: https://www.w3.org/TR/xml/
  title: "W3C, Extensible Markup Language (XML) 1.0 (Fifth Edition), W3C Recommendation, 26 November 2008"
  locator: Abstract; Status of this Document; 1.1 Origin and Goals; 1.2 Terminology; 2 Documents; 2.1 to 2.11; 3.1 Start-Tags, End-Tags, and Empty-Element Tags; 4.6 Predefined Entities; 5.1 Validating and Non-Validating Processors
- url: https://docs.python.org/3.14/library/xml.etree.elementtree.html
  title: The Python Standard Library, xml.etree.ElementTree, Python 3.14
  locator: Parsing XML; Modifying an XML File; iterparse; TreeBuilder Objects
- url: https://www.w3.org/TR/xml-names/
  title: "W3C, Namespaces in XML 1.0 (Third Edition), W3C Recommendation, 8 December 2009"
  locator: 1 Motivation and Summary; 3 Declaring Namespaces
- url: https://cheatsheetseries.owasp.org/cheatsheets/XML_External_Entity_Prevention_Cheat_Sheet.html
  title: OWASP Cheat Sheet Series, XML External Entity Prevention Cheat Sheet
  locator: Introduction; General Guidance; Java, DocumentBuilderFactory
- url: https://docs.python.org/3.14/library/xml.html
  title: The Python Standard Library, XML Processing Modules, Python 3.14
  locator: XML security
- url: https://pkg.go.dev/encoding/xml
  title: Go standard library, package encoding/xml
  locator: Overview; func Marshal; type Decoder (Strict); Bugs
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.xml/javax/xml/parsers/DocumentBuilderFactory.html
  title: Java SE 25 API, javax.xml.parsers.DocumentBuilderFactory
  locator: the class description and setFeature
- url: https://learn.microsoft.com/en-us/dotnet/api/system.xml.linq.xelement.parse
  title: .NET API documentation, XElement.Parse Method
  locator: the overloads and the remarks
---

## What it is

XML (Extensible Markup Language) is a text format for trees. A document is a tree of named elements. Each element can have attributes, text and child elements. The W3C defines the format in the Recommendation "Extensible Markup Language (XML) 1.0". The abstract says: "The Extensible Markup Language (XML) is a subset of SGML that is completely described in this document."[^1] Many document types and protocols are written in XML, for example XHTML, SVG, Atom feeds and the files of office suites.

The specification has two levels of correctness. "A data object is an XML document if it is well-formed, as defined in this specification. In addition, the XML document is valid if it meets certain further constraints."[^1] A well-formed document obeys the grammar and a short list of rules, such as the correct nesting of tags. A valid document also obeys a document type declaration (DTD) that gives the permitted elements and attributes. Most programs only need a well-formed document. A parser that finds an error of well-formedness has found a fatal error, and "the processor MUST NOT continue normal processing".[^1] An XML parser does not repair bad input, as an HTML parser does.

XML is a format for documents and for data. A document has mixed content, for example a paragraph with text and an emphasis element in the middle of the text. A data record has elements that hold only other elements or only text. The two uses have different needs, and the libraries of each language serve one of them better than the other (see Tools and libraries).

### History

The W3C formed the XML Working Group in 1996. It was "originally known as the SGML Editorial Review Board", and Jon Bosak of Sun Microsystems was its chair.[^1] The aim was to serve and process generic SGML on the Web "in the way that is now possible with HTML".[^1] The editors of the fifth edition are Tim Bray, Jean Paoli, C. M. Sperberg-McQueen, Eve Maler and François Yergeau.[^1] One of the ten design goals of the group shows the priority of the format: "Terseness in XML markup is of minimal importance."[^1]

## Syntax or structure

The specification gives the grammar in an extended BNF. The rules below are copied from the specification. `S` is whitespace, `Name` is a name and `Char` is any permitted character.[^1]

```ebnf
[1]  document     ::= prolog element Misc*
[22] prolog       ::= XMLDecl? Misc* (doctypedecl Misc*)?
[23] XMLDecl      ::= '<?xml' VersionInfo EncodingDecl? SDDecl? S? '?>'
[27] Misc         ::= Comment | PI | S
[39] element      ::= EmptyElemTag | STag content ETag
[40] STag         ::= '<' Name (S Attribute)* S? '>'
[41] Attribute    ::= Name Eq AttValue
[42] ETag         ::= '</' Name S? '>'
[43] content      ::= CharData? ((element | Reference | CDSect | PI | Comment) CharData?)*
[44] EmptyElemTag ::= '<' Name (S Attribute)* S? '/>'
[10] AttValue     ::= '"' ([^<&"] | Reference)* '"' | "'" ([^<&'] | Reference)* "'"
[14] CharData     ::= [^<&]* - ([^<&]* ']]>' [^<&]*)
[67] Reference    ::= EntityRef | CharRef
[68] EntityRef    ::= '&' Name ';'
[18] CDSect       ::= CDStart CData CDEnd
[15] Comment      ::= '<!--' ((Char - '-') | ('-' (Char - '-')))* '-->'
[3]  S            ::= (#x20 | #x9 | #xD | #xA)+
```

The grammar does not say all of the rules. The well-formedness constraints add more. The most important are these:

- **One root.** "There is exactly one element, called the root, or document element, no part of which appears in the content of any other element."[^1] Comments, processing instructions and whitespace can come before and after the root, but no text and no second element.
- **Matched tags.** The name in an end tag must be the same as the name in its start tag, and the elements must nest correctly. `<a><b></a></b>` is not well-formed.
- **Unique attributes.** "An attribute name MUST NOT appear more than once in the same start-tag or empty-element tag."[^1] The specification also says that "the order of attribute specifications in a start-tag or empty-element tag is not significant".[^1]
- **Escaped markup characters.** "The ampersand character (&) and the left angle bracket (<) MUST NOT appear in their literal form, except when used as markup delimiters, or within a comment, a processing instruction, or a CDATA section."[^1] In text, they are written `&amp;` and `&lt;`.

A document can use five entity references with no declaration: `&amp;`, `&lt;`, `&gt;`, `&apos;` and `&quot;`.[^1] A character reference gives a character by its code point, in decimal (`&#233;`) or in hexadecimal (`&#xE9;`). Any other entity, such as `&nbsp;` of HTML, needs a declaration in a DTD. A CDATA section begins with `<![CDATA[` and ends with `]]>`. Inside it, `<` and `&` are text, so a program can put code or another markup text there with no escapes.[^1]

Names are case-sensitive, so `<Book>` and `<book>` are two element types. Names that begin with `xml`, in any case, are reserved for the specifications of the W3C.[^1] An element with no content can be written as a start tag and an end tag, `<br></br>`, or as one empty-element tag, `<br/>`.

The XML declaration `<?xml version="1.0" encoding="UTF-8"?>` is optional. When it is present, it must be the first thing in the document. "All XML processors MUST accept the UTF-8 and UTF-16 encodings of Unicode".[^1] A document in another encoding must declare it. Before a parser does anything else, it changes each carriage return and each pair of a carriage return and a line feed to one line feed (#xA).[^1]

The specification defines two classes of processors. A validating processor reads the DTD and reports each violation of it. A non-validating processor need only check that the document is well-formed.[^1] Newer schema languages, such as XML Schema and RELAX NG, replace the DTD for most validation (see XML schema languages).

## Examples

The document below has each common part of the format: a declaration, a comment, a root element with an attribute, entity references and a CDATA section. The Python program reads it with `xml.etree.ElementTree`, then builds a new element and writes it.[^2]

```python run id=parse-and-write
import xml.etree.ElementTree as ET

text = """<?xml version="1.0" encoding="UTF-8"?>
<!-- a library with one book -->
<library city="Zürich">
  <book id="b1" lang="en">
    <title>Tom &amp; Jerry &lt;3</title>
    <note><![CDATA[a < b && b > c]]></note>
  </book>
</library>
"""
root = ET.fromstring(text)
print(root.tag, root.attrib)
book = root.find("book")
print(book.get("id"), book.get("lang"))
print(book.find("title").text)
print(book.find("note").text)
print(repr(root.text))

out = ET.Element("book", {"id": "b2"})
ET.SubElement(out, "title").text = "Q&A <draft>"
print(ET.tostring(out, encoding="unicode"))
```

```output for=parse-and-write
library {'city': 'Zürich'}
b1 en
Tom & Jerry <3
a < b && b > c
'\n  '
<book id="b2"><title>Q&amp;A &lt;draft&gt;</title></book>
```

The parser turns `&amp;` into `&` and gives the CDATA section as plain text, so the program sees the characters and not the markup. The tree has no trace of the comment, because the default tree builder drops comments.[^2] The `text` of `library` is a line feed and two spaces. Whitespace between tags is character data, and the parser keeps it. The writer escapes `&`, `<` and `>` in the text, so the output is well-formed again.

A parser must stop at the first error of well-formedness. Each document below breaks one rule of the previous section: matched tags, unique attributes, escaped markup characters and one root.

```python run id=not-well-formed
import xml.etree.ElementTree as ET

for text in ['<a><b></a></b>', '<a x="1" x="2"/>', '<a>Tom & Jerry</a>', '<a/><b/>']:
    try:
        ET.fromstring(text)
    except ET.ParseError as error:
        print("ParseError:", error)
```

```output for=not-well-formed
ParseError: mismatched tag: line 1, column 8
ParseError: duplicate attribute: line 1, column 9
ParseError: not well-formed (invalid token): line 1, column 8
ParseError: junk after document element: line 1, column 4
```

The example files show the same work in C#, Go and Java. C# uses LINQ to XML, Go maps elements to struct fields, and Java uses the DOM parser of the JAXP API.

## Versions

XML 1.0 has had five editions. An edition corrects the text and adds the errata. It is not a new version. The status section of the fifth edition says: "This fifth edition is not a new version of XML."[^1]

| Document | Date | Change |
|---|---|---|
| XML 1.0, Fourth Edition | 16 August 2006 | superseded by the fifth edition[^1] |
| XML 1.0, Fifth Edition | 26 November 2008 | erratum E09 permits many more characters in names; supersedes the fourth edition[^1] |
| Namespaces in XML 1.0, Third Edition | 8 December 2009 | all known errata to its date; supersedes the edition of 16 August 2006[^3] |

XML 1.1 is a separate version of the language, with its own Recommendation. The fifth edition of XML 1.0 took its main benefit into XML 1.0: erratum E09 "relaxes the restrictions on element and attribute names, thereby providing in XML 1.0 the major end user benefit currently achievable only by using XML 1.1."[^1]

Namespaces in XML is a separate specification on top of XML 1.0. It gives each element and attribute an expanded name, a namespace URI and a local name, so that two vocabularies can use the same local names in one document.[^3] Almost every modern XML document and library uses namespaces (see XML namespaces).

## Pitfalls

- **External entities (XXE).** A DTD can declare an entity whose value is the content of a file or a URL. "XML External Entity (XXE) injection occurs when an XML processor resolves an external entity from untrusted input. This can expose local files, cause server-side request forgery (SSRF), or exhaust resources."[^4] The OWASP advice is: "Disable document type definitions (DTDs) whenever possible."[^4] The Java example sets the feature `disallow-doctype-decl` for this reason. The defaults differ between parsers and between versions, so read the documentation of each parser.
- **Entity expansion (billion laughs).** An internal entity can refer to other entities, which refer to more entities, so a short DTD can expand to a very large text. The Python documentation says that "Expat versions lower than 2.7.2 may be vulnerable to the “billion laughs”, “quadratic blowup” and “large tokens” vulnerabilities".[^5] Reject DTDs in input from outside, or use a parser with expansion limits.
- **Whitespace is text.** The indentation between tags is character data. A program that reads the `text` of an element with children gets the line feeds and the spaces. A program that compares two documents byte by byte sees a difference when only the indentation differs.
- **Text and tail.** In mixed content, text appears between the child elements. ElementTree stores the text after a child in the `tail` attribute of that child, not in the parent.[^2] A program that reads only `text` loses the rest of the paragraph.
- **HTML entities.** `&nbsp;`, `&copy;` and the other HTML entities are not XML. A document that uses them with no DTD is not well-formed. Use the character reference, such as `&#160;`, or the character itself.
- **Encoding.** When no external information gives the encoding, such as an HTTP header, a document with no declaration must be UTF-8 or UTF-16.[^1] A file in Latin-1 with no declaration fails to parse at the first non-ASCII byte, or worse, reads as the wrong characters. Declare the encoding, or use UTF-8.
- **Attribute order.** The order of attributes is not significant.[^1] A writer can change it, and a program must not depend on it. Two documents with the same data can differ as text, so compare the trees, or use a canonical form.
- **String concatenation.** A program that makes XML with string formatting forgets to escape `&` and `<` in some value, and the output is not well-formed, or worse, the value adds elements. Build the tree with the library and let the writer escape the text.
- **Data binding loses structure.** The Go documentation says: "Mapping between XML elements and data structures is inherently flawed: an XML element is an order-dependent collection of anonymous values, while a data structure is an order-independent collection of named values."[^6] A struct cannot keep the order of different child elements, comments or mixed text. Use the token or tree API for documents.
- **Memory.** A tree parser holds the whole document in memory. For large files, use an event or pull parser. In Python, `iterparse` builds the tree as it reads, but "The tree is only built incrementally, it is not freed incrementally".[^2] The program must clear each element after it uses it (see Tree, event and pull parsers).

The program below shows how the Expat parser of Python treats entities. An internal entity from the DTD expands. An external entity does not load, and an HTML entity with no declaration is an error.

```python run id=entities
import xml.etree.ElementTree as ET

inner = '<!DOCTYPE a [<!ENTITY who "Jerry">]><a>Tom &amp; &who;</a>'
print(ET.fromstring(inner).text)

outer = '<!DOCTYPE a [<!ENTITY secret SYSTEM "file:///etc/passwd">]><a>&secret;</a>'
try:
    print(repr(ET.fromstring(outer).text))
except ET.ParseError as error:
    print("ParseError:", error)

try:
    ET.fromstring('<a>&nbsp;</a>')
except ET.ParseError as error:
    print("ParseError:", error)
```

```output for=entities
Tom & Jerry
ParseError: undefined entity &secret;: line 1, column 62
ParseError: undefined entity: line 1, column 3
```

"By default, Expat itself does not access local files or create network connections."[^5] Other parsers can load external entities. For example, the OWASP cheat sheet says that the .NET parser `XmlDocument` is vulnerable to external entity attacks below version 4.5.2 of the .NET Framework. The cheat sheet lists the safe settings for each parser.[^4]

## Tools and libraries

| Language | Verdict | Parse and write |
|---|---|---|
| Python | stdlib | `xml.etree.ElementTree`, `xml.dom.minidom`, `xml.sax`[^2] |
| Java | stdlib | JAXP: `DocumentBuilderFactory` (DOM), `SAXParserFactory`, `XMLInputFactory` (StAX), `Transformer`[^7] |
| C# | stdlib | LINQ to XML (`XElement.Parse`, `XDocument`), `XmlReader`, `XmlDocument`[^8] |
| Go | stdlib | `encoding/xml`: `Unmarshal`, `Marshal`, `Decoder.Token`[^6] |
| JavaScript, TypeScript | library | `DOMParser` and `XMLSerializer` in browsers; a package such as fast-xml-parser or xmldom in Node.js |
| Rust | library | quick-xml (pull parser), roxmltree (read-only tree) |
| C, C++ | library | libxml2, Expat; in C++ also pugixml and TinyXML-2 |
| Bash | library | the command-line tools `xmllint` (from libxml2) and `xmlstarlet` |

The Go package "implements a simple XML 1.0 parser that understands XML name spaces."[^6] Its decoder is strict by default. When the field `Strict` is false, it accepts some common errors, such as a missing end tag. With the maps `HTMLAutoClose` and `HTMLEntity`, it can then read some HTML.[^6]

The command-line tool `xmllint` checks that a file is well-formed, validates it against a DTD or a schema, and formats it. `xsltproc` applies an XSLT style sheet (see XSLT). To find nodes in a tree, most libraries support XPath (see XPath).

## References
