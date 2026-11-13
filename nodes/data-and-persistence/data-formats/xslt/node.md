---
tag: 6H9V1G
kind: standard
slug: xslt
name: XSLT
aliases: [XSL Transformations, XSLT 1.0, XSLT 3.0]
area: data-and-persistence/data-formats
level: advanced
summary: >-
  XSLT is a language for transforming XML documents into XML, HTML or text. A stylesheet is an XML document of template rules, and each rule uses XPath to match nodes and build the output.
see_also: [xml, xpath, xml-namespaces, read-and-write-xml, xml-schema-languages]
sources:
- url: https://developer.mozilla.org/en-US/docs/Web/XML/XSLT
  title: MDN Web Docs, XSLT, Extensible Stylesheet Language Transformations
  locator: the introduction
- url: https://developer.mozilla.org/en-US/docs/Web/XML/XSLT/Guides/Transforming_XML_with_XSLT
  title: MDN Web Docs, Transforming XML with XSLT
  locator: An overview; XPath and node selection
- url: https://developer.mozilla.org/en-US/docs/Web/XML/XSLT/Reference/Element
  title: MDN Web Docs, XSLT elements reference
  locator: Literal result elements; attribute value templates
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.xml/javax/xml/transform/TransformerFactory.html
  title: Java SE 25 API, Class TransformerFactory
  locator: newTransformer; newTemplates; setFeature; the access properties
- url: https://learn.microsoft.com/en-us/dotnet/standard/data/xml/xslt-transformations
  title: .NET documentation, XSLT Transformations
  locator: the introduction
- url: https://lxml.de/xpathxslt.html
  title: lxml documentation, XPath and XSLT with lxml
  locator: the introduction; XSLT
- url: https://www.saxonica.com/html/documentation12/xsl-elements/for-each-group.html
  title: Saxonica, Saxon 12 Documentation, xsl:for-each-group
  locator: the summary; Saxon availability
- url: https://www.saxonica.com/html/documentation12/about/index.html
  title: Saxonica, Saxon 12 Documentation, About Saxon
  locator: XML technologies
- url: https://developer.mozilla.org/en-US/docs/Web/API/XSLTProcessor
  title: MDN Web Docs, XSLTProcessor
  locator: the deprecation note
- url: https://learn.microsoft.com/en-us/dotnet/api/system.xml.xsl.xslcompiledtransform
  title: .NET API documentation, XslCompiledTransform Class
  locator: Remarks; Thread Safety
---

## What it is

XSLT (XSL Transformations) is a language that transforms XML documents. "Extensible Stylesheet Language Transformations (XSLT) is an XML -based language used, in conjunction with specialized processing software, for the transformation of XML documents."[^1] The input document does not change: "a new XML document is created based on the content of an existing document. Then, the new document may be serialized (output) by the processor in standard XML syntax or in another format, such as HTML or plain text."[^1]

A program in XSLT is called a stylesheet. "An XSLT stylesheet is an XML document."[^2] The stylesheet contains template rules: "an XSLT stylesheet is a set of rules, called templates, which declare that any node that matches this specific pattern should be manipulated in this specific way and end up in this specific position in the result tree."[^2] The patterns and the expressions in a stylesheet are written in XPath.

XSLT is declarative. A stylesheet does not loop over the input tree and call functions in a fixed order. It says what output to make for each kind of node, and the processor walks the tree and selects the rule for each node. "XSLT is most often used to convert data between different XML schemas or to convert XML data into web pages or PDF documents."[^1]

## Syntax or structure

A stylesheet is an `xsl:stylesheet` element (or its synonym `xsl:transform`) in the namespace `http://www.w3.org/1999/XSL/Transform`. The `version` attribute names the XSLT version. The main parts are:

- `xsl:output` sets the output method: `xml`, `html` or `text`.
- `xsl:template match="pattern"` is a template rule. Its content is the output for each matching node.
- `xsl:apply-templates select="expression"` selects nodes and processes each one with its matching template rule. `xsl:sort` inside it sets the order.
- `xsl:value-of select="expression"` writes the string value of an expression.
- `xsl:for-each`, `xsl:if` and `xsl:choose` are the loop and the conditions inside a template.
- `xsl:param` and `xsl:variable` bind names. A top-level `xsl:param` is a value that the calling program can set.
- `xsl:copy` and `xsl:copy-of` copy nodes of the input.

Elements in the template that are not in the XSLT namespace are literal result elements. The processor copies them to the output. An attribute of a literal result element can contain an attribute value template: "An attribute value template is a string that includes an embedded XPath expression which is used to specify the value of an attribute."[^3] "The expression to be evaluated is placed inside curly braces".[^3] For example, `<a href='/books/{@id}'>` makes a link from the `id` attribute.

When two rules match the same node, one of them is used: "In general, a more specific template rule takes precedence over a less specific one and, other things being equal, a template rule that appears later in the document takes precedence over one that appears earlier."[^2]

If no rule of the stylesheet matches a node, a built-in rule applies. For an element, the built-in rule processes the children. For a text node, it copies the text to the output. A stylesheet with no rules at all therefore writes all the text of the document.

## Examples

The JDK has an XSLT 1.0 processor in `javax.xml.transform`. The program below sorts books by year and writes one line for each book. The stylesheet has a parameter `from`. The program runs the compiled stylesheet twice: once without the parameter, so the default `0` applies, and once with `from` set to 1850.

```java run id=sort-books
import java.io.StringReader;
import java.io.StringWriter;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.stream.StreamResult;
import javax.xml.transform.stream.StreamSource;

public class Main {
    public static void main(String[] args) throws Exception {
        String xml = """
            <library>
              <book year="1871"><title>Middlemarch</title></book>
              <book year="1815"><title>Emma</title></book>
              <book year="1853"><title>Bleak House</title></book>
            </library>
            """;
        String xsl = """
            <xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
              <xsl:output method="text"/>
              <xsl:param name="from" select="0"/>
              <xsl:template match="/library">
                <xsl:apply-templates select="book[@year &gt;= $from]">
                  <xsl:sort select="@year" data-type="number"/>
                </xsl:apply-templates>
              </xsl:template>
              <xsl:template match="book">
                <xsl:value-of select="concat(@year, ' ', title, '&#10;')"/>
              </xsl:template>
            </xsl:stylesheet>
            """;
        var templates = TransformerFactory.newInstance().newTemplates(new StreamSource(new StringReader(xsl)));
        for (String from : new String[] {null, "1850"}) {
            var transformer = templates.newTransformer();
            if (from != null) transformer.setParameter("from", from);
            var out = new StringWriter();
            transformer.transform(new StreamSource(new StringReader(xml)), new StreamResult(out));
            System.out.print((from == null ? "no parameter" : "from = " + from) + "\n" + out);
        }
    }
}
```

```output for=sort-books
no parameter
1815 Emma
1853 Bleak House
1871 Middlemarch
from = 1850
1853 Bleak House
1871 Middlemarch
```

The rule for `/library` selects the books and sorts them. `data-type='number'` makes the sort numeric. Without it, the sort compares strings. The rule for `book` writes the line. The `&#10;` in the stylesheet is a newline character. `newTemplates` compiles the stylesheet once. A `Templates` object is "a compiled representation of the source",[^4] and the program makes a new `Transformer` from it for each run.

The next program has a rule for `title` only. The built-in rules process the `book` element and copy the text of `author`, which has no rule:

```java run id=builtin-rules
import java.io.StringReader;
import java.io.StringWriter;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.stream.StreamResult;
import javax.xml.transform.stream.StreamSource;

public class Main {
    public static void main(String[] args) throws Exception {
        String xml = "<book id=\"b1\"><title>Emma</title><author>Jane Austen</author></book>";
        String xsl = """
            <xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
              <xsl:output method="text"/>
              <xsl:template match="title">[<xsl:value-of select="."/>]</xsl:template>
            </xsl:stylesheet>
            """;
        var out = new StringWriter();
        TransformerFactory.newInstance().newTransformer(new StreamSource(new StringReader(xsl)))
                .transform(new StreamSource(new StringReader(xml)), new StreamResult(out));
        System.out.println(out);
    }
}
```

```output for=builtin-rules
[Emma]Jane Austen
```

The `id` attribute is not in the output. The built-in rule for an element processes its child nodes, and attributes are not children. To leave out the author, add an empty rule: `<xsl:template match="author"/>`.

A common pattern is the identity transform. One rule copies each node and each attribute and processes its contents. Other rules then change only some nodes. The program below removes the attribute `internal` and renames `author` to `creator`:

```java run id=identity
import java.io.StringReader;
import java.io.StringWriter;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.stream.StreamResult;
import javax.xml.transform.stream.StreamSource;

public class Main {
    public static void main(String[] args) throws Exception {
        String xml = "<book id=\"b1\" internal=\"x7\"><title>Emma</title><author>Jane Austen</author></book>";
        String xsl = """
            <xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
              <xsl:output method="xml" omit-xml-declaration="yes"/>
              <xsl:template match="@* | node()">
                <xsl:copy><xsl:apply-templates select="@* | node()"/></xsl:copy>
              </xsl:template>
              <xsl:template match="@internal"/>
              <xsl:template match="author">
                <creator><xsl:apply-templates select="@* | node()"/></creator>
              </xsl:template>
            </xsl:stylesheet>
            """;
        var out = new StringWriter();
        TransformerFactory.newInstance().newTransformer(new StreamSource(new StringReader(xsl)))
                .transform(new StreamSource(new StringReader(xml)), new StreamResult(out));
        System.out.println(out);
    }
}
```

```output for=identity
<book id="b1"><title>Emma</title><creator>Jane Austen</creator></book>
```

The pattern `@* | node()` matches all attributes and all child nodes. The empty rule for `@internal` writes nothing, so the attribute is dropped. Both specific rules take precedence over the general copy rule.

## Versions

| Version | Notes |
|---|---|
| XSLT 1.0 | the version of the Java, .NET and libxslt processors[^4][^5][^6] |
| XSLT 2.0 | grouping with `xsl:for-each-group`, which is "Available in XSLT 2.0 and later versions."[^7] |
| XSLT 3.0 | the current W3C Recommendation; implemented by Saxon 12[^8] |

Most processors in standard libraries implement only XSLT 1.0. In Java, `newTransformer` takes "an XSLT document that conforms to XSL Transformations (XSLT) Version 1.0".[^4] In .NET, "The XslCompiledTransform class supports the W3C XSLT 1.0 recommendation."[^5] "lxml supports XPath 1.0, XSLT 1.0 and the EXSLT extensions through libxml2 and libxslt in a standards compliant way."[^6] EXSLT is a set of extension functions for XSLT 1.0 processors, for example for regular expressions and sets.

For XSLT 2.0 and 3.0, the usual processor is Saxon. "Saxon 12 includes highly conformant implementations of the current W3C Recommendations: XSLT 3.0, XQuery 3.1, XPath 3.1, and XSD 1.1."[^8] "Support for older specifications such as XSLT 2.0 and XQuery 1.0 has been dropped, but of course the newer specifications have a high level of backwards compatibility so existing code should continue to run without any problems."[^8] "XSLT 2.0 and 3.0 define a "backwards compatibility mode" for running XSLT 1.0 stylesheets".[^8]

In web browsers, XSLT is deprecated. MDN marks `XSLTProcessor` as deprecated and says: "XSLT is a candidate for removal from browsers because browser implementations are highly susceptible to memory safety vulnerabilities."[^9] Do new transformations on the server or in the build, not in the browser.

## Pitfalls

- **Built-in rules.** Text that no rule handles still goes to the output, as the second example shows. If the output has unexpected text, look for elements without a rule.
- **Namespaces in patterns.** In XSLT 1.0, a name without a prefix in a pattern matches only an element in no namespace. If the input uses a default namespace, declare a prefix for that namespace in the stylesheet and write `match="b:book"`.
- **String or number.** `xsl:sort` sorts strings unless `data-type="number"` is set. Without it, `1000` sorts before `999`.
- **The version gap.** A stylesheet written for XSLT 2.0 or 3.0 uses functions and instructions, such as `xsl:for-each-group`, that an XSLT 1.0 processor does not have. Check which version your processor implements before you copy a stylesheet.
- **Thread safety.** In Java, "Care must be taken not to use this Transformer in multiple Thread s running concurrently."[^4] Share a `Templates` object and create a `Transformer` for each thread. In .NET, "The XslCompiledTransform object is thread safe once it has been loaded."[^10]
- **Untrusted stylesheets.** A stylesheet is a program. It can read other documents with the `document()` function, include other stylesheets, and in some processors call extension functions. In Java, `xsl:import` and `xsl:include` can only use the protocols of the property `XMLConstants.ACCESS_EXTERNAL_STYLESHEET`, "Access to external document through XSLT document function is restricted",[^4] and the feature `XMLConstants.FEATURE_SECURE_PROCESSING` makes the processor "behave in a secure fashion as defined by the implementation".[^4] In .NET, "Support for the XSLT document() function and embedded script blocks are disabled by default."[^10] Do not run a stylesheet from an untrusted source with these features turned on.

## Tools and libraries

| Tool | Versions | Notes |
|---|---|---|
| Java `javax.xml.transform` | XSLT 1.0 | `TransformerFactory`, `Templates` and `Transformer`[^4] |
| .NET `System.Xml.Xsl` | XSLT 1.0 | `XslCompiledTransform`; "The Load method loads and compiles the style sheet, while the Transform method executes the XSLT transform."[^10] |
| libxslt and `xsltproc` | XSLT 1.0 and EXSLT | the C library under lxml[^6] |
| Python lxml | XSLT 1.0 and EXSLT | `etree.XSLT`; "By default, XSLT supports all extension functions from libxslt and libexslt as well as Python regular expressions through the EXSLT regexp functions ."[^6] |
| Saxon | XSLT 3.0 | Java, .NET, and C/C++, PHP and Python editions; "The Home Edition remains open source (under the Mozilla Public License 2.0)."[^8] |
| Web browsers | | `XSLTProcessor`, deprecated[^9] |

## References
