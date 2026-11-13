---
tag: 307PT1
kind: standard
slug: xpath
name: XPath
aliases: [XML Path Language, XPath 1.0, XPath 3.1]
area: data-and-persistence/data-formats
level: core
summary: >-
  XPath is a W3C expression language that selects nodes of an XML tree with paths of steps, such as /library/book[@year > 2000]/title, and computes strings, numbers and booleans.
see_also: [xml, xml-namespaces, jsonpath, tree-event-and-pull-parsers]
sources:
- url: https://www.w3.org/TR/xpath-31/
  title: "W3C, XML Path Language (XPath) 3.1, W3C Recommendation, 21 March 2017"
  locator: Introduction; Document Order; Path Expressions; Steps; Abbreviated Syntax; Incompatibilities relative to XPath 1.0
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.xml/javax/xml/xpath/package-summary.html
  title: Java SE 25 API, package javax.xml.xpath
  locator: 1 XPath Overview; 2 XPath Expressions; 3 XPath Data Types; 4 XPath Context
- url: https://developer.mozilla.org/en-US/docs/Web/XML/XPath/Reference/Axes
  title: MDN Web Docs, XPath, Axes
  locator: the list of the thirteen axes
- url: https://learn.microsoft.com/en-us/dotnet/api/system.xml.xpath.extensions.xpathevaluate
  title: .NET API documentation, Extensions.XPathEvaluate Method
  locator: Returns; Remarks
- url: https://www.w3.org/TR/2014/REC-xpath-30-20140408/
  title: "W3C, XML Path Language (XPath) 3.0, W3C Recommendation, 8 April 2014"
  locator: Change Log
- url: https://docs.python.org/3.14/library/xml.etree.elementtree.html
  title: The Python Standard Library, xml.etree.ElementTree, Python 3.14
  locator: XPath support; Supported XPath syntax
- url: https://developer.mozilla.org/en-US/docs/Web/XML/XPath
  title: MDN Web Docs, XPath
  locator: the introduction
---

## What it is

XPath (XML Path Language) is a language for expressions that select parts of an XML document. "The primary purpose of XPath is to address the nodes of XML trees and JSON trees."[^1] An expression such as `/library/book[@year > 2000]/title` reads like a file path: it starts at the root, goes down to each `book` child of `library`, keeps the books whose `year` attribute is greater than 2000, and selects their `title` children. An expression can also compute a value, such as `count(//book)` or `sum(//price)`.

XPath is not a complete language. "XPath is designed to be embedded in a host language".[^1] The host language, such as XSLT, XQuery, XML Schema or a programming language API, sets the context of the expression and uses its result. "XPath uses a compact, non-XML syntax to facilitate use of XPath within URIs and XML attribute values."[^1]

Two versions are in wide use. XPath 1.0 is the version of most standard libraries and of the browsers. The Java API says: "The XPath API supports XML Path Language (XPath) Version 1.0".[^2] XPath 2.0, 3.0 and 3.1 add sequences, the types of XML Schema, many more functions and, in 3.1, maps and arrays for JSON. Few libraries outside the XSLT and XQuery processors implement them.

## Syntax or structure

A location path is a sequence of steps separated by `/`. Each step has three parts: an axis, a node test and zero or more predicates. In the full syntax, `child::book[attribute::year > 2000]` is a step on the `child` axis, with the node test `book` and one predicate. The step selects the nodes on the axis that pass the node test, and then keeps the nodes for which each predicate is true.

An axis is a direction in the tree from the context node. "There are thirteen different axes in the XPath specification."[^3]

| Axis | Selects |
|---|---|
| `child` | the children of the context node; the default axis |
| `descendant`, `descendant-or-self` | the children, their children, and so on (and the node itself) |
| `parent` | the parent of the context node |
| `ancestor`, `ancestor-or-self` | the parent, its parent, and so on up to the root (and the node itself) |
| `following-sibling`, `preceding-sibling` | the nodes with the same parent, after or before the context node |
| `following`, `preceding` | all nodes after or before the context node in the document, but not the descendants or the ancestors |
| `attribute` | the attributes of the context node |
| `namespace` | the namespace nodes of the context node |
| `self` | the context node |

The node test is a name, such as `book`, a wildcard `*`, or a kind test, such as `text()`, `comment()` and `node()`. A predicate is an expression in square brackets. A number in a predicate is a position: `book[2]` is the second `book` child. Any other value is converted to a boolean: `book[price]` keeps the books that have a `price` child, and `book[@year > 2000]` keeps the books with a year after 2000.

The abbreviated syntax makes paths short. XPath 3.1 gives the rules.[^1]

| Abbreviation | Full form |
|---|---|
| `book` | `child::book` |
| `@id` | `attribute::id` |
| `//` | `/descendant-or-self::node()/` |
| `.` | `self::node()` (the context item) |
| `..` | `parent::node()` |
| `book[2]` | `child::book[position() = 2]` |

An expression that starts with `/` starts at the root of the document. An expression with no `/` at the start is relative to the context node. XPath 1.0 has four types of values: the node-set, the boolean, the number (a binary64 double) and the string.[^2] It has operators for arithmetic (`+`, `-`, `*`, `div`, `mod`), comparisons (`=`, `!=`, `<`, `<=`, `>`, `>=`), `and`, `or` and the union of node-sets `|`, and a library of functions, such as `count`, `sum`, `contains`, `starts-with`, `substring`, `normalize-space`, `string-length`, `name` and `not`.

The nodes of a node-set have an order. "Informally, document order is the order in which nodes appear in the XML serialization of a document."[^1] A program that reads a result gets the nodes in document order, even for a reverse axis such as `preceding-sibling`. The .NET documentation notes this: "this extension method returns nodes in document order."[^4]

## Examples

The Java program below evaluates six paths over a small document. The text of each selected `p` element is printed after the path.

```java run id=abbreviations
import java.io.StringReader;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathConstants;
import javax.xml.xpath.XPathFactory;
import org.w3c.dom.Document;
import org.w3c.dom.NodeList;
import org.xml.sax.InputSource;

public class Main {
    public static void main(String[] args) throws Exception {
        String text = "<doc><sec><p>a</p><p>b</p></sec><sec><p>c</p></sec></doc>";
        Document doc = DocumentBuilderFactory.newInstance().newDocumentBuilder()
                .parse(new InputSource(new StringReader(text)));
        XPath xp = XPathFactory.newInstance().newXPath();
        String[] paths = {
            "//p[1]",
            "/descendant::p[1]",
            "(//p)[1]",
            "/descendant-or-self::node()/child::p[1]",
            "//sec[p = 'c']/p",
            "//p[. = 'b']/preceding-sibling::p",
        };
        for (String path : paths) {
            NodeList nodes = (NodeList) xp.evaluate(path, doc, XPathConstants.NODESET);
            StringBuilder out = new StringBuilder();
            for (int i = 0; i < nodes.getLength(); i++) {
                out.append(nodes.item(i).getTextContent());
            }
            System.out.printf("%-42s %s%n", path, out);
        }
    }
}
```

```output for=abbreviations
//p[1]                                     ac
/descendant::p[1]                          a
(//p)[1]                                   a
/descendant-or-self::node()/child::p[1]    ac
//sec[p = 'c']/p                           c
//p[. = 'b']/preceding-sibling::p          a
```

The first and the second paths look alike, but they differ. The specification says: "The path expression //para[1] does not mean the same as the path expression /descendant::para[1]. The latter selects the first descendant para element; the former selects all descendant para elements that are the first para children of their respective parents."[^1] The fourth path is the full form of the first, and it gives the same result. The parentheses of the third path apply the predicate to the whole node-set. The fifth path selects all `p` elements of each `sec` that has a `p` child with the text `c`.

The example files show XPath in Java, C# and Python: node selection, the functions `count` and `sum`, and the comparisons with node-sets.

## Versions

| Version | Date | Main changes |
|---|---|---|
| XPath 1.0 | 1999 | paths, the four types and the core functions; made for XSLT 1.0 and XPointer[^2] |
| XPath 3.0 | 8 April 2014 | named function references, inline function expressions, dynamic function calls, the string concatenation operator `\|\|` and the simple map operator `!`[^5] |
| XPath 3.1 | 21 March 2017 | maps and arrays, and functions for JSON[^1] |

The Java documentation says: "XPath started in life in 1999 as a supplement to the XSLT and XPointer languages, but has more recently become popular as a stand-alone language, as a single XPath expression can be used to replace many lines of DOM API code."[^2]

XPath 2.0 came between 1.0 and 3.0. In XPath 2.0, every item is either a node or an atomic value, and XPath 3.0 adds function items to this model.[^5] The later versions are not fully compatible with XPath 1.0. XPath 3.1 has an XPath 1.0 compatibility mode, and its appendix lists the differences that remain. For example: "Consecutive comparison operators such as A < B < C were supported in XPath 1.0, but are not permitted by the XPath 3.1 grammar."[^1] Also, "10div 3 was permitted in XPath 1.0, but in XPath 3.1 must be written as 10 div 3."[^1] The namespace axis is deprecated: "Implementations may support the namespace axis for backward compatibility with XPath 1.0, but they are not required to do so."[^1]

## Pitfalls

- **Default namespaces.** In XPath 1.0, a name with no prefix in an expression is in no namespace. In a document with `xmlns="..."` on the root, the path `/library/book` selects nothing. Bind a prefix to the namespace in the API, such as a `NamespaceContext` in Java, and write `/b:library/b:book`. The prefix of the expression need not be the prefix of the document.
- **Comparisons with node-sets.** `//book/@year = 2008` is true if any year is 2008, and `//book/@year != 2008` is true if any year is not 2008. Both can be true at the same time, as the Java example shows. To test that no node has a value, use `not(... = ...)`.
- **`//p[1]` is not the first `p`.** The predicate applies to each step, so `//p[1]` is the first `p` child of each parent. Use `(//p)[1]` for the first `p` in the document.
- **Numbers are doubles.** In XPath 1.0, every number is a binary64 double. Most decimal fractions, such as 0.1, have no exact binary64 value, so the `sum` of decimal prices can differ from the exact decimal total in the last digits. A text that is not a number becomes `NaN`, with no error.
- **Whitespace and mixed text.** `text()` selects each text node, and an element with a comment or a child element in the middle of its text has more than one text node. The string value of an element, as in `string(title)` or `title = 'Beta'`, joins all of its descendant text. Use `normalize-space()` to ignore the spaces at the ends.
- **`//` at the start is slow.** `//book` searches the whole document. On a large tree, a full path such as `/library/book` visits fewer nodes.
- **XPath injection.** A program that makes an expression with string concatenation from user input, such as `"//user[name='" + name + "']"`, lets the user change the query. A value with a quote breaks out of the string literal. Use variables of the API, such as an `XPathVariableResolver` in Java, or escape the quotes.
- **Subset implementations.** Python `ElementTree` supports only "a small subset of the abbreviated syntax".[^6] An expression that works in Java or in a browser can raise a `SyntaxError` there.

## Tools and libraries

| Language | Verdict | API |
|---|---|---|
| Java | stdlib | `javax.xml.xpath`, XPath 1.0 over DOM[^2] |
| C# | stdlib | `XPathSelectElements` and `XPathEvaluate` on LINQ to XML, `XPathNavigator`; XPath 1.0[^4] |
| Python | stdlib (subset) | `ElementTree.find` and `findall`[^6]; full XPath 1.0 in the library lxml |
| JavaScript | native in browsers | `XPathExpression`[^7] and `Document.evaluate`; a library in Node.js |
| C | library | libxml2 `xmlXPathEvalExpression` |
| Go, Rust | library | no XPath in the standard library |

The command-line tool `xmllint --xpath` of libxml2 evaluates an XPath 1.0 expression on a file. For XPath 3.1, use an XSLT 3.0 or XQuery processor such as Saxon or BaseX. MDN notes that XPath is "mainly used in XSLT" and can also navigate "the DOM of any XML-like language document", such as HTML and SVG.[^7]

## References
