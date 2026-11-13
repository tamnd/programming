---
tag: RNZSX1
kind: concept
slug: xml-namespaces
name: XML namespaces
aliases: [Namespaces in XML, xmlns]
area: data-and-persistence/data-formats
level: advanced
summary: >-
  An XML namespace is a URI that qualifies element and attribute names, so that two vocabularies can use the same local name in one document without a clash.
see_also: [xml, tree-event-and-pull-parsers, xpath]
sources:
- url: https://www.w3.org/TR/xml-names/
  title: "W3C, Namespaces in XML 1.0 (Third Edition), W3C Recommendation, 8 December 2009"
  locator: 1 Motivation and Summary; 2.1 Basic Concepts; 2.2 Use of URIs as Namespace Names; 2.3 Comparing URI References; 3 Declaring Namespaces; 6.1 Namespace Scoping; 6.2 Namespace Defaulting; 6.3 Uniqueness of Attributes
- url: https://www.w3.org/TR/xml/
  title: "W3C, Extensible Markup Language (XML) 1.0 (Fifth Edition), W3C Recommendation, 26 November 2008"
  locator: 2.3 Common Syntactic Constructs (the note on the colon)
- url: https://docs.python.org/3.14/library/xml.etree.elementtree.html
  title: The Python Standard Library, xml.etree.ElementTree, Python 3.14
  locator: Parsing XML with Namespaces; register_namespace
- url: https://pkg.go.dev/encoding/xml
  title: Go standard library, package encoding/xml
  locator: type Name; func Marshal; func Unmarshal
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.xml/javax/xml/parsers/DocumentBuilderFactory.html
  title: Java SE 25 API, javax.xml.parsers.DocumentBuilderFactory
  locator: setNamespaceAware
- url: https://learn.microsoft.com/en-us/dotnet/standard/linq/namespaces-overview
  title: .NET documentation, Namespaces overview (LINQ to XML)
  locator: the whole article
---

## Idea

An XML document can mix elements from more than one vocabulary. A book catalog can use its own elements for the books, the Dublin Core elements for the metadata and the XHTML elements for a description. Two vocabularies can use the same name for different things, for example `title` for the title of a book and `title` for the title of an HTML page. A program must know which `title` it has.

Namespaces in XML, a W3C Recommendation, solves this problem. "An XML namespace is identified by a URI reference".[^1] Each element and attribute name is a pair: "An expanded name is a pair consisting of a namespace name and a local name."[^1] In the text, a short prefix stands for the URI, so the document says `m:lang` and not the full URI each time. The prefix is only a local abbreviation. The expanded name is the real name.

The motivation is modularity: "if such a markup vocabulary exists which is well-understood and for which there is useful software available, it is better to re-use this markup rather than re-invent it."[^1]

## How it works

A namespace declaration is an attribute. The attribute `xmlns:p="URI"` binds the prefix `p` to the URI. The attribute `xmlns="URI"` sets the default namespace. The document below uses both.

```xml
<catalog xmlns="urn:example:books" xmlns:m="urn:example:meta">
  <book id="b1" m:lang="en">
    <title>Tide tables</title>
    <price xmlns="urn:example:money" currency="EUR">32.18</price>
  </book>
</catalog>
```

The rules that give each name its namespace are these:

- **Scope.** A declaration applies from the start tag where it appears to the end of that element, "excluding the scope of any inner declarations with the same NSAttName part".[^1] In the example, `price` declares a new default namespace, so `price` is in `urn:example:money`, and `title` is still in `urn:example:books`.
- **Prefixed names.** An element or attribute name with a prefix is in the namespace that the prefix is bound to. `m:lang` is the attribute `lang` in `urn:example:meta`. A prefix that has no declaration in scope is an error.
- **Unprefixed elements.** "A default namespace declaration applies to all unprefixed element names within its scope."[^1] `catalog`, `book` and `title` are in `urn:example:books`.
- **Unprefixed attributes.** "Default namespace declarations do not apply directly to attribute names".[^1] "The namespace name for an unprefixed attribute name always has no value."[^1] So `id` and `currency` are in no namespace, even though their elements are in a namespace.
- **Undeclaring.** The attribute `xmlns=""` removes the default namespace for an element and its contents.[^1] A prefix cannot be undeclared in Namespaces in XML 1.0.

The two prefixes `xml` and `xmlns` are reserved. "The prefix xml is by definition bound to the namespace name http://www.w3.org/XML/1998/namespace."[^1] It is the namespace of the attributes `xml:lang` and `xml:space`. The prefix `xmlns` is only for declarations, and a document must not declare it.[^1]

The namespace name is an identifier, not an address. It "SHOULD have the characteristics of uniqueness and persistence. It is not a goal that it be directly usable for retrieval of a schema (if any exists)."[^1] A parser never fetches the URI. A URN such as `urn:example:books` and a URL such as `http://www.w3.org/1999/xhtml` work in the same way. "The empty string, though it is a legal URI reference, cannot be used as a namespace name."[^1]

Two namespace names are the same only when they are the same string. "The comparison is case-sensitive, and no %-escaping is done or undone."[^1] `http://example.com/a` and `HTTP://example.com/a` are two different namespaces, though they are the same web address.

Namespaces change the rules of uniqueness of attributes. A tag must not have two attributes with the same expanded name. "This constraint is equivalent to requiring that no element have two attributes with the same expanded name".[^1] So `a:x` and `b:x` are an error when the prefixes `a` and `b` are bound to the same URI.

The specification is a layer on top of XML 1.0. XML 1.0 itself only reserves the colon for this use: "authors should not use the colon in XML names except for namespace purposes, but XML processors must accept the colon as a name character."[^2] A parser that is not namespace-aware sees `m:lang` as one name with a colon in it.

## In languages

The XML libraries of each language give the expanded name in their own form. The Python example below shows the Clark notation `{namespace}local` of ElementTree.[^3]

```python run id=prefixes-do-not-matter
import xml.etree.ElementTree as ET

first = ET.fromstring('<x:item xmlns:x="urn:example:books"/>')
second = ET.fromstring('<item xmlns="urn:example:books"/>')
third = ET.fromstring('<item xmlns="urn:Example:books"/>')
print(first.tag, second.tag, first.tag == second.tag)
print(third.tag, first.tag == third.tag)

try:
    ET.fromstring('<x:item/>')
except ET.ParseError as error:
    print("ParseError:", error)
```

```output for=prefixes-do-not-matter
{urn:example:books}item {urn:example:books}item True
{urn:Example:books}item False
ParseError: unbound prefix: line 1, column 0
```

The first two documents use a prefix and a default namespace, and they give the same expanded name. The third differs only in the case of one letter, and it is a different namespace. The last document uses a prefix with no declaration, and the parser rejects it.

| Language | Expanded name | Find by namespace |
|---|---|---|
| Python | the string `{uri}local`[^3] | `find("p:book", {"p": uri})` |
| Go | `xml.Name{Space, Local}`[^4] | the struct tag `xml:"uri local"` |
| Java | `getNamespaceURI()` and `getLocalName()` on a namespace-aware DOM[^5] | `getElementsByTagNameNS(uri, local)` |
| C# | `XName`, made with `XNamespace + "local"`[^6] | `Element(ns + "book")` |

The Java `DocumentBuilderFactory` is not namespace-aware by default: "By default the value of this is set to false".[^5] A program must call `setNamespaceAware(true)`, or the parser treats `m:lang` as a name with a colon and gives no namespace. The example files show each language.

## Variations and related ideas

- **XPath and XSLT.** An XPath expression uses prefixes too, but the prefixes of the expression are bound by the program, not by the document. A query for `/catalog/book` finds nothing in a document with a default namespace. The query must use a prefix bound to the namespace, such as `/b:catalog/b:book`.
- **Namespaces of schemas.** A schema in XML Schema describes the elements of one target namespace. The namespace URI is the key that links a document to its schema.
- **JSON-LD and RDF.** The `@context` of JSON-LD and the prefixes of Turtle use the same idea: a short prefix stands for a long IRI.
- **Namespaces in programming languages.** A package or a module name in a programming language is a namespace for the names of the code. An XML namespace does the same for the names of the markup.

## Pitfalls

- **Searching with the local name only.** In a document with a default namespace, every unprefixed element is in that namespace. A search for `book` with no namespace finds nothing. This is a common bug. The fix is to search with the expanded name.
- **Depending on the prefix.** The prefix is not part of the name. Another writer can use `x:item`, `item` with a default namespace, or `ns0:item` for the same element. A program that compares the prefix or the qualified name `m:lang` as a string breaks when the prefix changes.
- **Unprefixed attributes are in no namespace.** A program that looks for the attribute `{urn:example:books}id` finds nothing, because `id` has no prefix.
- **Prefixes in the output.** Some writers do not keep the prefixes of the input. ElementTree writes `ns0` and `ns1` unless the program calls `register_namespace`, and that registry "is global".[^3] Go writes an `xmlns` attribute on each element and invents prefixes for attributes. The output is correct but hard to read. When people read the output, set the prefixes.
- **Prefixes inside values.** Some vocabularies, such as XML Schema and XSLT, put qualified names inside attribute values, for example `type="xs:string"`. A writer that changes the prefixes does not change these values, and the document breaks. Keep the original prefixes when you edit such a document.
- **Case and spelling of the URI.** The comparison is a plain string comparison.[^1] A trailing slash, a different case or a different scheme makes a different namespace.
- **The namespace URI is not a link.** A program must not fetch the URI to read the document. The URI need not point to anything.[^1]

## Cost

Namespaces add a small cost to the parse: the parser keeps a stack of the bindings in scope and looks up each prefix. A tree keeps a namespace name with each element. The larger cost is for people: the expanded names make queries and programs longer.

## History

The first edition of Namespaces in XML came out in 1999. The XML 1.0 specification lists it in its references as "Namespaces in XML", by Tim Bray, Dave Hollander and Andrew Layman, World Wide Web Consortium, 1999.[^2] The third edition of 8 December 2009 "incorporates all known errata as of the publication date" and supersedes the edition of 16 August 2006.[^1]

## References
