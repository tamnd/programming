---
tag: WXK4C9
kind: task
slug: read-and-write-xml
name: Read and write XML
aliases: [parse and serialize XML, XML round trip, canonicalize XML]
area: data-and-persistence/data-formats
level: core
summary: >-
  Parse an XML text with the parser of the standard library, then write the root element back in a fixed form that follows Canonical XML, so that texts with the same data give the same output.
see_also: [xml, xml-namespaces, tree-event-and-pull-parsers, query-xml-with-xpath, read-and-write-json]
sources:
- url: https://www.rfc-editor.org/rfc/rfc3076.txt
  title: "RFC 3076, Canonical XML Version 1.0, J. Boyer, March 2001"
  locator: Abstract; 1.1 Terminology; 2.2 Document Order; 2.3 Processing Model
- url: https://docs.python.org/3.14/library/xml.dom.minidom.html
  title: The Python Standard Library, xml.dom.minidom, Python 3.14
  locator: parseString; Node.toxml
- url: https://docs.python.org/3.14/library/xml.etree.elementtree.html
  title: The Python Standard Library, xml.etree.ElementTree, Python 3.14
  locator: canonicalize; register_namespace; ElementTree.write (short_empty_elements)
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.xml/javax/xml/parsers/DocumentBuilderFactory.html
  title: Java SE 25 API, javax.xml.parsers.DocumentBuilderFactory
  locator: newDefaultInstance; setFeature; setNamespaceAware
- url: https://cheatsheetseries.owasp.org/cheatsheets/XML_External_Entity_Prevention_Cheat_Sheet.html
  title: OWASP Cheat Sheet Series, XML External Entity Prevention Cheat Sheet
  locator: Introduction; General Guidance; Java, DocumentBuilderFactory (DOM)
- url: https://learn.microsoft.com/en-us/dotnet/api/system.xml.xmldocument.preservewhitespace
  title: .NET API documentation, XmlDocument.PreserveWhitespace Property
  locator: Property Value; Remarks
- url: https://learn.microsoft.com/en-us/dotnet/api/system.xml.xmlreadersettings.dtdprocessing
  title: .NET API documentation, XmlReaderSettings.DtdProcessing Property
  locator: Property Value; Remarks
- url: https://pkg.go.dev/encoding/xml
  title: Go standard library, package encoding/xml
  locator: Decoder.RawToken; Decoder.Strict; Directive; Encoder.EncodeToken
- url: https://docs.python.org/3.14/library/xml.html
  title: The Python Standard Library, XML Processing Modules, Python 3.14
  locator: XML security
---

## Task

Parse an XML text into a tree, then write the tree back as text. Each solution is a function `xml_rewrite(text)` that parses the text with the XML parser of its standard library and writes the root element back in one fixed form. The parser does the reading, and the solution does the writing with its own code, so that the output does not depend on the defaults of a library serializer.

Two texts that differ only in ways that XML permits give the same output. Such differences are the quote characters around an attribute value, the order of the attributes, `<item/>` against `<item></item>`, a CDATA section against escaped text, and a character reference against the character itself. A test can compare two documents in this way, and a program can check that its writer keeps the data of the tree.

The fixed form follows the rules of Canonical XML. RFC 3076 gives the purpose: "if two documents have the same canonical form, then the two documents are logically equivalent within the given application context."[^1] The rules that the contract uses are these:

- An element is written as a start tag, its content and an end tag. "Empty elements are converted to start-end tag pairs"[^1], so `<item/>` becomes `<item></item>`.
- Each attribute is written as a space, the qualified name, `=` and the value in double quotes. The namespace declarations come first, and "the default namespace node, if one exists, has no local name and is therefore lexicographically least"[^1]. The other declarations follow in the order of their prefixes, and then the other attributes in the order of their qualified names. The order compares code points.
- In an attribute value, the solution replaces "all ampersands (&) with &amp;, all open angle brackets (<) with &lt;, all quotation mark (double quote) characters with &quot;, and the whitespace characters #x9, #xA, and #xD, with character references"[^1]. The references are `&#x9;`, `&#xA;` and `&#xD;`.
- In text, "all ampersands are replaced by &amp;, all open angle brackets (<) are replaced by &lt;, all closing angle brackets (>) are replaced by &gt;, and all #xD characters are replaced by &#xD;."[^1] Each other character is written as itself, also when it is not ASCII.
- The parser replaces the references and removes the CDATA markers: "Character and parsed entity references are replaced" and "CDATA sections are replaced with their character content"[^1]. The parser also changes each line end to a line feed: "Line breaks normalized to #xA on input, before parsing"[^1].
- The XML declaration, comments, processing instructions and all content outside the root element are not written.

The contract is not full Canonical XML. Canonical XML sorts the attributes "with namespace URI as the primary key and local name as the secondary key"[^1], and the contract sorts them by qualified name. The two orders are the same when no attribute has a prefix. Canonical XML also keeps the processing instructions, can keep the comments, and removes the namespace declarations that an ancestor already makes.

The solution reports `parse-error` when the text is not well-formed XML or when it has a document type declaration (`<!DOCTYPE`).

## Solutions

Python, Java, C# and Go have an XML parser in the standard library. The standard libraries of JavaScript (Node.js), TypeScript, Rust, C, C++ and Bash do not, and `matrix.yaml` names the usual libraries.

- **Python.** `xml.dom.minidom.parseString` returns "a Document that represents the string"[^2], and the solution walks the DOM tree. It does not use ElementTree, because ElementTree does not keep the prefixes of the text (see Pitfalls). Python also has `ElementTree.canonicalize`, a "C14N 2.0 transformation function"[^3]. Its output is close to the contract, but it keeps processing instructions and drops the declarations of prefixes that no name uses:

```python run id=canonicalize
import xml.etree.ElementTree as ET

for text in ['<a v="&#9;">x<!-- note -->y<?app data?>z</a>', '<r xmlns="urn:d" xmlns:m="urn:m" b="2" a="1"/>']:
    print(ET.canonicalize(text))
```

```output for=canonicalize
<a v="&#x9;">xy<?app data?>z</a>
<r xmlns="urn:d" a="1" b="2"></r>
```

  The contract gives `<a v="&#x9;">xyz</a>` and `<r xmlns="urn:d" xmlns:m="urn:m" a="1" b="2"></r>` for these two texts.
- **Java.** The solution makes the parser with `DocumentBuilderFactory.newDefaultInstance()`, which "Creates a new instance of the DocumentBuilderFactory builtin system-default implementation."[^4] It sets `setNamespaceAware(true)`, the feature `disallow-doctype-decl` and `FEATURE_SECURE_PROCESSING`, as the OWASP cheat sheet shows for the DOM parser[^5]. The DOM gives the namespace declarations as attributes, and `Text` and `CDATASection` nodes for the text.
- **C#.** The solution loads an `XmlDocument` from an `XmlReader`. It sets `PreserveWhitespace` to true, because with the default of false, "significant white space is preserved, white space is not."[^6] The `DtdProcessing` of the reader settings is `Prohibit`, which is already the default: "The default is Prohibit."[^7] With this value, the reader throws "an XmlException exception when a DTD is encountered."[^7]
- **Go.** The package `encoding/xml` has a token decoder. The solution uses `Decoder.RawToken`, because it keeps the prefixes of the text. "RawToken is like Decoder.Token but does not verify that start and end elements match and does not translate name space prefixes to their corresponding URLs."[^8] The solution therefore keeps a stack of open elements and checks each end tag. It also checks for a duplicate attribute, a second root element and text after the root, because the decoder accepts these (see Pitfalls). A token of type `Directive` "represents an XML directive of the form <!text>"[^8], and the solution refuses it, because a document type declaration is such a directive.

All four solutions refuse a document type declaration, because entity declarations in it can read local files or make the parser use much memory (see Pitfalls).

## Edge cases

The cases are in `cases/basic.jsonl` and `cases/edge.jsonl`.

- `declaration-and-comment` and `comment-and-pi-inside`: the XML declaration, comments and processing instructions are not written. The text on both sides of a comment joins, so `x<!-- note -->y` becomes `xy`.
- `empty-element`: `<item/>` and `<item></item>` give the same output.
- `attributes-sorted`, `single-quotes` and `attribute-whitespace`: the attributes are sorted, single quotes become double quotes, and the spaces in a value are kept.
- `escapes-in-text`, `escapes-in-attribute` and `greater-than-in-attribute`: `>` is escaped in text but not in a value, and `"` is escaped in a value but not in text. `&apos;` becomes `'`.
- `character-references`: `&#9;&#10;&#13;` in a value stay as references, in the hexadecimal form, and `&#65;&#x42;` in text become `AB`.
- `cdata`: the content of a CDATA section is written as escaped text.
- `whitespace-kept`, `mixed-content` and `crlf`: all white space in the content is kept, and each carriage return and line feed pair becomes a line feed.
- `non-ascii`: the characters are written as themselves, so `&#x1F600;` becomes the emoji.
- `default-namespace`, `prefixes-kept`, `declarations-first` and `inner-declaration`: the prefixes of the text are kept, and the namespace declarations come before the other attributes.

`cases/errors.jsonl` lists the texts that each solution must refuse with `parse-error`: tags that do not match, a missing end tag, an empty text, two root elements, text after the root, a bare `&`, an entity that is not declared, a `<` in a value, a duplicate attribute and two texts with a document type declaration. The toolchain does not load this file yet, so each test file holds these cases by name.

## Pitfalls

- **Library serializers change the text.** ElementTree gives its own prefixes to the namespaces, unless a program calls `register_namespace`: then "Tags and attributes in this namespace will be serialized with the given prefix, if at all possible."[^3] Its writer also writes an element with no content as one tag by default: "If True (the default), they are emitted as a single self-closed tag, otherwise they are emitted as a pair of start/end tags."[^3] The minidom writer keeps the prefixes but also writes `<x:item n="1"/>`:

```python run id=serializers
import xml.etree.ElementTree as ET
from xml.dom import minidom

text = '<x:list xmlns:x="urn:example:x"><x:item n="1"/></x:list>'
print(ET.tostring(ET.fromstring(text), encoding="unicode"))
print(minidom.parseString(text).documentElement.toxml())
print(ET.canonicalize(text))
```

```output for=serializers
<ns0:list xmlns:ns0="urn:example:x"><ns0:item n="1" /></ns0:list>
<x:list xmlns:x="urn:example:x"><x:item n="1"/></x:list>
<x:list xmlns:x="urn:example:x"><x:item n="1"></x:item></x:list>
```

  All three texts are the same XML data. Compare documents in one fixed form, and not in the output of a serializer.
- **Copying Go tokens.** `Decoder.Token` gives each name the namespace URL in `Space`: "Each of the Name structures contained in the Token has the Space set to the URL identifying its name space when known."[^8] An `Encoder` that writes these tokens back does not get the prefixes back:

```go run id=go-round-trip
package main

import (
	"encoding/xml"
	"fmt"
	"io"
	"strings"
)

func main() {
	text := "<x:list xmlns:x=\"urn:example:x\"><x:item a=\"1\ttab\"/></x:list>"
	dec := xml.NewDecoder(strings.NewReader(text))
	var out strings.Builder
	enc := xml.NewEncoder(&out)
	for {
		tok, err := dec.Token()
		if err == io.EOF {
			break
		}
		if err != nil {
			fmt.Println(err)
			return
		}
		if err := enc.EncodeToken(tok); err != nil {
			fmt.Println(err)
			return
		}
	}
	enc.Flush()
	fmt.Println(out.String())
}
```

```output for=go-round-trip
<list xmlns="urn:example:x" xmlns:_xmlns="xmlns" _xmlns:x="urn:example:x"><item xmlns="urn:example:x" a="1&#x9;tab"></item></list>
```

  The output also shows that the Go decoder keeps a literal tab in an attribute value. The parsers of Python and Java change it to a space, as Canonical XML expects: "Attribute values are normalized, as if by a validating processor"[^1]. For this reason the contract has no literal tab, line feed or carriage return in a value.
- **Go accepts some texts that are not XML.** The decoder of `encoding/xml` reads a duplicate attribute, a second root element and text after the root with no error. Both `Token` and `RawToken` accept them:

```go run id=go-not-checked
package main

import (
	"encoding/xml"
	"fmt"
	"io"
	"strings"
)

func main() {
	for _, text := range []string{"<a x=\"1\" x=\"2\"/>", "<a/><b/>", "<a/>tail", "<a>&nbsp;</a>"} {
		dec := xml.NewDecoder(strings.NewReader(text))
		var err error
		for err == nil {
			_, err = dec.Token()
		}
		if err == io.EOF {
			fmt.Printf("%-20q accepted\n", text)
		} else {
			fmt.Printf("%-20q %v\n", text, err)
		}
	}
}
```

```output for=go-not-checked
"<a x=\"1\" x=\"2\"/>" accepted
"<a/><b/>"           accepted
"<a/>tail"           accepted
"<a>&nbsp;</a>"      XML syntax error on line 1: invalid character entity &nbsp;
```

  The Go solution checks these three cases itself.
- **External entities.** OWASP describes the attack: "XML External Entity (XXE) injection occurs when an XML processor resolves an external entity from untrusted input. This can expose local files, cause server-side request forgery (SSRF), or exhaust resources."[^5] The general advice is: "Disable document type definitions (DTDs) whenever possible. Reject DOCTYPE declarations if the parser supports it."[^5] The JDK parser with its default settings reads the file that an entity names:

```java run id=external-entity
import java.io.StringReader;
import java.nio.file.Files;
import java.nio.file.Path;
import javax.xml.parsers.DocumentBuilderFactory;
import org.xml.sax.InputSource;
import org.xml.sax.SAXParseException;
import org.xml.sax.helpers.DefaultHandler;

public class Main {
    static void parse(DocumentBuilderFactory factory, String text) throws Exception {
        var builder = factory.newDocumentBuilder();
        builder.setErrorHandler(new DefaultHandler());
        try {
            var doc = builder.parse(new InputSource(new StringReader(text)));
            System.out.println("parsed: " + doc.getDocumentElement().getTextContent());
        } catch (SAXParseException e) {
            System.out.println("refused: " + e.getClass().getSimpleName());
        }
    }

    public static void main(String[] args) throws Exception {
        Path secret = Files.createTempFile("secret", ".txt");
        Files.writeString(secret, "the content of a local file");
        String text = "<!DOCTYPE a [<!ENTITY e SYSTEM \"" + secret.toUri() + "\">]><a>&e;</a>";

        // The default settings read the file that the entity names.
        parse(DocumentBuilderFactory.newDefaultInstance(), text);

        DocumentBuilderFactory safe = DocumentBuilderFactory.newDefaultInstance();
        safe.setFeature("http://apache.org/xml/features/disallow-doctype-decl", true);
        parse(safe, text);
        Files.delete(secret);
    }
}
```

```output for=external-entity
parsed: the content of a local file
refused: SAXParseException
```

  Python is safer by default: "By default, Expat itself does not access local files or create network connections."[^9] But Expat still expands the entities that a document type declaration defines, and "Expat versions lower than 2.7.2 may be vulnerable to the “billion laughs”, “quadratic blowup” and “large tokens” vulnerabilities"[^9]. For this reason the Python solution also refuses a document type declaration. In .NET, `DtdProcessing.Prohibit` is the default[^7].
- **White space in .NET.** An `XmlDocument` with the default `PreserveWhitespace` of false drops the white space between elements[^6]. Set it to true before `Load`, or the output loses the line ends and the indentation of the text.

## Related tasks

- [[query-xml-with-xpath]] finds parts of an XML document with a path.
- [[read-and-write-json]] does the same job for JSON.
- [[validate-data-against-a-schema]] checks that a document has the structure that a schema describes.

## References
