---
tag: 8K94Q0
kind: task
slug: query-xml-with-xpath
name: Query XML with XPath
aliases: [XPath query, select XML nodes, find elements in XML]
area: data-and-persistence/data-formats
level: core
summary: >-
  Parse an XML text, evaluate an XPath path from the root element with a map of namespace prefixes, and return the string value of each selected element in document order.
see_also: [xpath, xml, xml-namespaces, tree-event-and-pull-parsers, jsonpath]
sources:
- url: https://docs.python.org/3.14/library/xml.etree.elementtree.html
  title: The Python Standard Library, xml.etree.ElementTree, Python 3.14
  locator: XPath support; Supported XPath syntax; Element.findall
- url: https://lxml.de/xpathxslt.html
  title: lxml, XPath and XSLT with lxml
  locator: The xpath() method; Namespaces and prefixes
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.xml/javax/xml/xpath/package-summary.html
  title: Java SE 25 API, package javax.xml.xpath
  locator: 1 XPath Overview; 3 XPath Data Types
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.xml/javax/xml/xpath/XPath.html
  title: Java SE 25 API, javax.xml.xpath.XPath
  locator: the interface description (QNames; thread safety); setNamespaceContext; setXPathVariableResolver
- url: https://learn.microsoft.com/en-us/dotnet/api/system.xml.xpath.extensions.xpathselectelements
  title: .NET API documentation, Extensions.XPathSelectElements Method
  locator: XPathSelectElements(XNode, String, IXmlNamespaceResolver)
- url: https://learn.microsoft.com/en-us/dotnet/api/system.xml.xmlreadersettings.dtdprocessing
  title: .NET API documentation, XmlReaderSettings.DtdProcessing Property
  locator: Property Value
- url: https://owasp.org/www-community/attacks/XPATH_Injection
  title: OWASP Community, XPATH Injection
  locator: Description; XPath Injection Defenses
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.xml/javax/xml/parsers/DocumentBuilderFactory.html
  title: Java SE 25 API, javax.xml.parsers.DocumentBuilderFactory
  locator: setNamespaceAware
---

## Task

Find parts of an XML document with an XPath expression. Each solution is a function `query_xpath(text, path, namespaces)` that parses the text, evaluates the path with the root element as the context node, and returns the string value of each selected element, in document order.

The path is in the abbreviated syntax of XPath 1.0 (see XPath). The contract uses the part of the syntax that all three standard libraries on this page support:

- `book/title` selects the `title` children of the `book` children of the root, and `.//title` selects all `title` elements under the root. A `*` as the last step selects all child elements.
- A step can have one predicate: `[@year]` and `[@year='2008']` test an attribute, `[note]` and `[title='Beta']` test a child, `[.='Beta']` tests the element itself, and `[2]` and `[last()]` select by position among the children of each parent.
- A name with a prefix, such as `m:lang`, is in the namespace that the map `namespaces` gives for the prefix. A name with no prefix is in no namespace.

The string value of an element is all the text inside it, also the text of its child elements. For `<title>Be<i>ta</i></title>` it is `Beta`. Entity references such as `&amp;` are replaced, and comments are not part of it.

The solution reports `parse-error` when the text is not well-formed XML, and `bad-path` when the path is not valid or uses a prefix that is not in the map.

## Solutions

Python, Java and C# have XPath in the standard library. The standard libraries of Go, JavaScript (Node.js), TypeScript, Rust, C, C++ and Bash do not, and `matrix.yaml` names the usual libraries.

- **Python.** `ElementTree.findall` takes a path and a namespace map. The documentation is clear about the limits: "The goal is to support a small subset of the abbreviated syntax; a full XPath engine is outside the scope of the module."[^1] `findall` "Returns a list containing all matching elements in document order", and its `namespaces` argument "is an optional mapping from namespace prefix to full name."[^1] The solution joins `itertext()` to get the string value. For full XPath 1.0 in Python, use the library lxml: its `xpath()` method takes a `namespaces` dictionary that "maps the namespace prefixes used in the XPath expression to namespace URIs"[^2].
- **Java.** "The XPath API supports XML Path Language (XPath) Version 1.0"[^3]. The solution parses with a namespace-aware `DocumentBuilderFactory` and evaluates the path with `XPath.evaluate` and `XPathConstants.NODESET`. A `NamespaceContext` gives the prefixes: "QNames in the expression are resolved against the XPath namespace context set with setNamespaceContext(NamespaceContext nsContext)."[^4] `getTextContent` gives the string value.
- **C#.** The extension method `XPathSelectElements` on LINQ to XML takes an `IXmlNamespaceResolver` "for the namespace prefixes in the XPath expression"[^5], and an `XmlNamespaceManager` is one. `XElement.Value` gives the string value.

All three solutions refuse a document type declaration. Java needs the feature `disallow-doctype-decl` for this. In .NET, the default of `XmlReaderSettings.DtdProcessing` already "is Prohibit"[^6].

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`. Most cases use one small library document with three books, where the third book is in a `shelf` element.

- `first-of-each-parent` and `last-of-each-parent`: `.//book[1]` gives the first `book` of the root and the first `book` of the shelf, not one book. The position counts among the children of each parent.
- `attribute-not-equal`: `[@m:lang!='en']` is false for a book with no `m:lang` attribute, because a comparison with an empty node-set is false.
- `has-attribute` and `has-child`: a predicate with only a name tests that the attribute or the child exists.
- `self`, `cdata-and-entities` and `comment-in-text`: the string value joins the text of child elements, replaces `&lt;` and `&amp;`, keeps the text of a CDATA section and drops comments.
- `default-namespace-unprefixed`, `default-namespace-prefixed` and `other-prefix`: in a document with a default namespace, `book` selects nothing and `b:book` with `b` bound to the namespace selects the book. The prefix in the path need not be the prefix in the document.
- `empty-elements`, `no-match`, `whitespace` and `non-ascii`: an empty element has the value `""`, a path that matches nothing gives `[]`, spaces are kept, and the values can hold any Unicode text.

`cases/errors.jsonl` lists the texts that are not XML and the paths that are not valid. The toolchain does not load this file yet, so each test file holds these cases by name.

## Pitfalls

- **Building the path from user input.** OWASP describes the attack: "Similar to SQL Injection, XPath Injection attacks occur when a web site uses user-supplied information to construct an XPath query for XML data."[^7] The defense is the same as for SQL: "use a parameterized XPath interface if one is available, or escape the user input"[^7]. In Java, a variable such as `$name` in the path takes its value from an `XPathVariableResolver`[^4], and lxml takes variables as keyword arguments[^2]:

```java run id=xpath-injection
import java.io.StringReader;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathConstants;
import javax.xml.xpath.XPathFactory;
import org.w3c.dom.Document;
import org.w3c.dom.NodeList;
import org.xml.sax.InputSource;

public class Main {
    static void show(String label, NodeList nodes) {
        StringBuilder out = new StringBuilder(label + ":");
        for (int i = 0; i < nodes.getLength(); i++) {
            out.append(' ').append(nodes.item(i).getTextContent());
        }
        System.out.println(out);
    }

    public static void main(String[] args) throws Exception {
        String text = "<users><user><name>ada</name><role>admin</role></user>"
                + "<user><name>bob</name><role>guest</role></user></users>";
        Document doc = DocumentBuilderFactory.newInstance().newDocumentBuilder()
                .parse(new InputSource(new StringReader(text)));
        XPath xp = XPathFactory.newInstance().newXPath();
        String name = "x' or 'a'='a";

        // Wrong: the input becomes part of the expression.
        String built = "/users/user[name='" + name + "']/role";
        System.out.println(built);
        show("concatenated", (NodeList) xp.evaluate(built, doc, XPathConstants.NODESET));

        // Right: the input is the value of the variable $name.
        xp.setXPathVariableResolver(q -> q.getLocalPart().equals("name") ? name : null);
        show("variable", (NodeList) xp.evaluate("/users/user[name=$name]/role", doc, XPathConstants.NODESET));
    }
}
```

```output for=xpath-injection
/users/user[name='x' or 'a'='a']/role
concatenated: admin guest
variable:
```

- **A parser that is not namespace-aware.** The Java `DocumentBuilderFactory` is not namespace-aware: "By default the value of this is set to false"[^8]. Without `setNamespaceAware(true)`, the elements have no namespace, and a path with a prefix finds nothing.
- **Default namespaces.** A path with `book` finds nothing in a document with a default namespace. Bind a prefix to the namespace and write `b:book`. In Python, a map entry with the empty prefix also works for ElementTree: "Pass '' as prefix to move all unprefixed tag names in the expression into the given namespace."[^1] This is an extension of ElementTree, and XPath 1.0 has no such rule.
- **ElementTree errors.** ElementTree raises `SyntaxError` for most bad paths, but a predicate with no closing bracket gives a `TypeError` in Python 3.12 and 3.14:

```python run id=elementtree-errors
import xml.etree.ElementTree as ET

root = ET.fromstring("<a><b/></a>")
for path in ["b[0]", "/a/b", "b[position()=1]", "b["]:
    try:
        print(path, root.findall(path))
    except Exception as error:
        print(path, type(error).__name__)
```

```output for=elementtree-errors
b[0] SyntaxError
/a/b SyntaxError
b[position()=1] SyntaxError
b[ TypeError
```

  The solution catches both. A path that is valid XPath 1.0 but outside the subset, such as `/a/b` or `b[position()=1]`, also fails in ElementTree. For an attribute value test, "The value cannot contain quotes."[^1]
- **Shared XPath objects in Java.** "An XPath object is not thread-safe and not reentrant."[^4] Make one for each thread, or one for each call, as the solution does.
- **`text()` is not the string value.** In full XPath, `title/text()` selects the text nodes, and `<title>Be<i>ta</i></title>` has only the text node `Be`. The string value `Beta` joins all the descendant text.

## Related tasks

- [[read-and-write-xml]] parses XML to a tree and writes it back.
- [[validate-data-against-a-schema]] checks that a document has the structure that a schema describes.

## References
