---
tag: TYAGG1
kind: standard
slug: xml-schema-languages
name: XML schema languages
aliases: [XSD, W3C XML Schema, RELAX NG, Schematron, DTD]
area: data-and-persistence/data-formats
level: advanced
summary: >-
  An XML schema language describes the documents that a program accepts. DTD, W3C XML Schema (XSD) and RELAX NG are grammars, and Schematron checks rules written in XPath.
see_also: [xml, xml-namespaces, xpath, json-schema, validate-data-against-a-schema]
sources:
- url: https://relaxng.org/jclark/design.html
  title: James Clark, The Design of RELAX NG
  locator: Evolution of DTDs; Declarations and definitions; Closure; XML syntax; Attributes; Infoset modification; Datatyping; Unordered content; Identity constraints; Associating schemas with documents
- url: https://relaxng.org/
  title: RELAX NG home page
  locator: the introduction; Specification; Software
- url: https://schematron.com/
  title: Schematron home page
  locator: the introduction
- url: https://relaxng.org/spec-20011203.html
  title: "OASIS, RELAX NG Specification, Committee Specification, 3 December 2001"
  locator: Abstract
- url: https://relaxng.org/compact-20021121.html
  title: "OASIS, RELAX NG Compact Syntax, Committee Specification, 21 November 2002"
  locator: Abstract; Introduction
- url: https://xerces.apache.org/xerces2-j/
  title: Apache Xerces2 Java Parser
  locator: the introduction
- url: https://docs.oracle.com/en/java/javase/25/docs/api/java.xml/javax/xml/validation/SchemaFactory.html
  title: Java SE 25 API, Class SchemaFactory
  locator: URIs for Supported Schema languages; newInstance
- url: https://lxml.de/validation.html
  title: lxml documentation, Validation with lxml
  locator: the introduction; DTD; Schematron; (Pre-ISO-Schematron)
- url: https://learn.microsoft.com/en-us/dotnet/api/system.xml.schema.xmlschemaset
  title: .NET API documentation, XmlSchemaSet Class
  locator: the summary
- url: https://learn.microsoft.com/en-us/dotnet/api/system.xml.schema.xmlschemaunique
  title: .NET API documentation, XmlSchemaUnique Class
  locator: the summary
---

## What it is

An XML schema is a description of a set of XML documents. It says which elements and attributes a document can contain, in which order, and which values they can have. A validator reads a document and a schema and tells whether the document matches. A program that validates its input before it reads the data does not need to check the structure in its own code.

There are several schema languages, and each has a different model:

- A **DTD** (document type definition) is part of XML 1.0. It describes the elements and the attributes with a grammar. "XML DTDs have a built-in, limited, rather ad hoc set of datatypes, which can be applied only to the values of attributes and not to the content of elements."[^1]
- **W3C XML Schema**, also called XSD, is a W3C language. Its schemas are XML documents. It adds a large set of datatypes, type derivation, default values and identity constraints such as unique keys.
- **RELAX NG** is a grammar language that is simpler than XSD. "RELAX NG is an evolution and generalization of XML DTDs [ 3 ]. It shares the same grammar-based paradigm."[^1] It "has both an XML syntax and a compact non-XML syntax".[^2]
- **Schematron** is not a grammar. It is "a simple but powerful language for asserting or reporting the presence or absence of arbitrary patterns in data, in particular using XPath over XML documents (but not limited to either)".[^3]

The grammar languages answer the question "does this tree have the right shape?" Schematron answers questions about relations between values, such as "is the minimum of each range smaller than its maximum?" The two kinds are often used together: a grammar checks the structure first, and Schematron then checks the business rules.

A schema does not say how a program gets it. "RELAX NG does not define any mechanism for associating a document with a RELAX NG schema."[^1] With DTDs, "the XML document uses a DOCTYPE declaration to identify the DTD with respect to which it is valid."[^1] The design essay of RELAX NG says that a receiver of a document cannot trust that declaration: "The recipient cannot assume that the DOCTYPE declaration of the document correctly identifies that DTD."[^1] A program that validates input should use its own copy of the schema.

## Syntax or structure

The same small document type is shown below in four forms. A `book` element has a required `id` attribute and two child elements, `title` and `year`, in this order.

A DTD uses its own syntax, not XML:

```text
<!ELEMENT book (title, year)>
<!ATTLIST book id ID #REQUIRED>
<!ELEMENT title (#PCDATA)>
<!ELEMENT year (#PCDATA)>
```

The DTD cannot say that `year` is a year. The content of an element is only text.

W3C XML Schema uses XML. The elements of the language are in the namespace `http://www.w3.org/2001/XMLSchema`, usually with the prefix `xs`:

```xml
<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">
  <xs:element name="book">
    <xs:complexType>
      <xs:sequence>
        <xs:element name="title" type="xs:string"/>
        <xs:element name="year" type="xs:gYear"/>
      </xs:sequence>
      <xs:attribute name="id" type="xs:ID" use="required"/>
    </xs:complexType>
  </xs:element>
</xs:schema>
```

XSD separates element declarations (`xs:element`) from type definitions (`xs:complexType`, `xs:simpleType`). "W3C XML Schema [ 24 ] distinguishes between definitions and declarations."[^1] A type can be named and reused, and a new type can extend or restrict a named type.

RELAX NG also has an XML syntax. A schema is a pattern, and the pattern elements nest in the same way as the document:

```xml
<element name="book" xmlns="http://relaxng.org/ns/structure/1.0"
         datatypeLibrary="http://www.w3.org/2001/XMLSchema-datatypes">
  <attribute name="id"><data type="ID"/></attribute>
  <element name="title"><text/></element>
  <element name="year"><data type="gYear"/></element>
</element>
```

"A RELAX NG schema specifies a pattern for the structure and content of an XML document. A RELAX NG schema is itself an XML document."[^4] "RELAX NG has, in the terminology of W3C XML Schema, only definitions."[^1] RELAX NG does not define its own datatypes. A schema names a datatype library, and "the datatypes defined by W3C XML Schema Part 2 [ 1 ] can be used as a datatype library".[^1]

The compact syntax of RELAX NG writes the same pattern in one line:

```text
element book { attribute id { xsd:ID }, element title { text }, element year { xsd:gYear } }
```

"This document specifies a compact, non-XML syntax for [ RELAX NG ]."[^5] The compact syntax needs no declaration for the XSD datatypes: "the xsd prefix is predeclared to the above URI."[^5] The URI is `http://www.w3.org/2001/XMLSchema-datatypes`.

Schematron uses XML. "A Schematron schema has: patterns , which have rules , which make assertions".[^3] The `context` of a rule is an XPath pattern that selects the nodes, and the `test` of an assertion is an XPath expression that must be true for each of them:

```xml
<schema xmlns="http://purl.oclc.org/dsdl/schematron">
  <pattern>
    <rule context="range">
      <assert test="number(@min) &lt;= number(@max)">The minimum must not be larger than the maximum.</assert>
    </rule>
  </pattern>
</schema>
```

The output of a Schematron validator is a report: "validating a document against a Schematron document produces an SVRL (Schematron Validation Report Language) document".[^3]

## Examples

The JDK has a validator for W3C XML Schema in the package `javax.xml.validation`. The program below compiles the XSD schema of the section above and validates three documents. The second document has a `year` that is not a year. The third document has no `id`, and its child elements are in the wrong order.

```java run id=xsd-check
import java.io.StringReader;
import javax.xml.XMLConstants;
import javax.xml.transform.stream.StreamSource;
import javax.xml.validation.SchemaFactory;
import org.xml.sax.SAXException;

public class Main {
    public static void main(String[] args) throws Exception {
        String xsd = """
            <xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">
              <xs:element name="book">
                <xs:complexType>
                  <xs:sequence>
                    <xs:element name="title" type="xs:string"/>
                    <xs:element name="year" type="xs:gYear"/>
                  </xs:sequence>
                  <xs:attribute name="id" type="xs:ID" use="required"/>
                </xs:complexType>
              </xs:element>
            </xs:schema>
            """;
        var factory = SchemaFactory.newInstance(XMLConstants.W3C_XML_SCHEMA_NS_URI);
        var validator = factory.newSchema(new StreamSource(new StringReader(xsd))).newValidator();
        for (String doc : new String[] {
                "<book id=\"b1\"><title>XML</title><year>2001</year></book>",
                "<book id=\"b1\"><title>XML</title><year>last year</year></book>",
                "<book><year>2001</year><title>XML</title></book>"}) {
            try {
                validator.validate(new StreamSource(new StringReader(doc)));
                System.out.println("valid");
            } catch (SAXException e) {
                System.out.println(e.getMessage());
            }
        }
    }
}
```

```output for=xsd-check
valid
cvc-datatype-valid.1.2.1: 'last year' is not a valid value for 'gYear'.
cvc-complex-type.4: Attribute 'id' must appear on element 'book'.
```

Without an error handler, `validate` throws an exception at the first error, so the program reports only the missing attribute for the third document.

XSD validation can change the document. The schema below gives the attribute `currency` a default value. The program parses a document without the attribute, validates it into a `DOMResult`, and reads the attribute again:

```java run id=xsd-default
import java.io.StringReader;
import javax.xml.XMLConstants;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.dom.DOMResult;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamSource;
import javax.xml.validation.SchemaFactory;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.xml.sax.InputSource;

public class Main {
    public static void main(String[] args) throws Exception {
        String xsd = """
            <xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">
              <xs:element name="price">
                <xs:complexType>
                  <xs:simpleContent>
                    <xs:extension base="xs:decimal">
                      <xs:attribute name="currency" type="xs:string" default="EUR"/>
                    </xs:extension>
                  </xs:simpleContent>
                </xs:complexType>
              </xs:element>
            </xs:schema>
            """;
        var dbf = DocumentBuilderFactory.newDefaultInstance();
        dbf.setNamespaceAware(true);
        Document doc = dbf.newDocumentBuilder().parse(new InputSource(new StringReader("<price>9.50</price>")));
        System.out.println("before: [" + doc.getDocumentElement().getAttribute("currency") + "]");
        var validator = SchemaFactory.newInstance(XMLConstants.W3C_XML_SCHEMA_NS_URI)
                .newSchema(new StreamSource(new StringReader(xsd))).newValidator();
        DOMResult result = new DOMResult();
        validator.validate(new DOMSource(doc), result);
        Element out = ((Document) result.getNode()).getDocumentElement();
        System.out.println("after:  [" + out.getAttribute("currency") + "]");
    }
}
```

```output for=xsd-default
before: []
after:  [EUR]
```

The data that the program reads after validation is not the data in the file. RELAX NG does not do this on purpose: "The omission of support for default attributes from RELAX NG is part of a general policy in RELAX NG of not modifying or augmenting the infoset".[^1] "RELAX NG validation does not involve changing the information about the document that is passed to an application."[^1]

XSD can also state identity constraints, which a grammar cannot express. The `xs:unique` element below says that no two `book` children of a `library` have the same `isbn`. The `selector` and the `field` are XPath expressions.

```java run id=xsd-unique
import java.io.StringReader;
import javax.xml.XMLConstants;
import javax.xml.transform.stream.StreamSource;
import javax.xml.validation.SchemaFactory;
import org.xml.sax.SAXException;

public class Main {
    public static void main(String[] args) throws Exception {
        String xsd = """
            <xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">
              <xs:element name="library">
                <xs:complexType>
                  <xs:sequence>
                    <xs:element name="book" maxOccurs="unbounded">
                      <xs:complexType>
                        <xs:attribute name="isbn" type="xs:string" use="required"/>
                      </xs:complexType>
                    </xs:element>
                  </xs:sequence>
                </xs:complexType>
                <xs:unique name="one-isbn">
                  <xs:selector xpath="book"/>
                  <xs:field xpath="@isbn"/>
                </xs:unique>
              </xs:element>
            </xs:schema>
            """;
        var validator = SchemaFactory.newInstance(XMLConstants.W3C_XML_SCHEMA_NS_URI)
                .newSchema(new StreamSource(new StringReader(xsd))).newValidator();
        for (String doc : new String[] {
                "<library><book isbn=\"1\"/><book isbn=\"2\"/></library>",
                "<library><book isbn=\"1\"/><book isbn=\"1\"/></library>"}) {
            try {
                validator.validate(new StreamSource(new StringReader(doc)));
                System.out.println("valid");
            } catch (SAXException e) {
                System.out.println(e.getMessage());
            }
        }
    }
}
```

```output for=xsd-unique
valid
cvc-identity-constraint.4.1: Duplicate unique value [1] declared for identity constraint "one-isbn" of element "library".
```

"RELAX NG itself provides no support for identity constraints."[^1] A RELAX NG user puts such rules in Schematron or in the program.

The `xmllint` tool of libxml2 validates with RELAX NG, XSD and Schematron. The commands below use the RELAX NG schema of the section above (`book.rng`) and the Schematron rule (`range.sch`). The output is from libxml2 2.9.13. The second book has no `id` and the wrong order.

```text
$ xmllint --noout --relaxng book.rng ok.xml bad.xml
ok.xml validates
bad.xml:1: element year: Relax-NG validity error : Element book failed to validate attributes
bad.xml:1: element year: Relax-NG validity error : Did not expect element year there
bad.xml fails to validate
```

For the Schematron rule and the document `<ranges><range min="1" max="5"/><range min="9" max="2"/></ranges>`, a validator reports the second range. The `xmllint` of libxml2 2.9.13 implements only the old Schematron namespace `http://www.ascc.net/xml/schematron`, so the check below used a copy of the rule in that namespace:

```text
$ xmllint --noout --schematron range.sch ranges.xml
/ranges/range[2] line 1: The minimum must not be larger than the maximum.
ranges.xml fails to validate
```

The page on validating data against a schema has a C# program that validates XML with XSD through `XmlSchemaSet`.

## Versions

| Language | Version | Notes |
|---|---|---|
| DTD | XML 1.0 | defined in the XML specification itself; datatypes only for attribute values[^1] |
| W3C XML Schema | 1.0 (2001) | Part 1 Structures and Part 2 Datatypes; the datatypes are also used by RELAX NG[^1] |
| W3C XML Schema | 1.1 | Xerces2 has a processor for it[^6] |
| RELAX NG | Committee Specification, 3 December 2001 | OASIS; based on RELAX and TREX[^4] |
| RELAX NG compact syntax | Committee Specification, 21 November 2002 | OASIS; editor James Clark[^5] |
| RELAX NG | ISO/IEC 19757-2:2003, second edition 2008 | Part 2 of DSDL[^2] |
| Schematron | ISO/IEC 19757-3, fourth edition | Part 3 of DSDL[^3] |

"RELAX NG was based on TREX designed by James Clark and RELAX Core designed by MURATA Makoto."[^2] "RELAX NG is also an International Standard (ISO/IEC 19757-2). It is Part 2 of ISO/IEC 19757 DSDL (Document Schema Definition Languages)".[^2] Schematron is "an ISO/IEC International Standard, one of the Document Schema Description Languages (DSDL) standards".[^3] Its home page says: "The fourth edition of the ISO Schematron standard has now been published , twenty years after the first edition."[^3]

Many libraries implement only XSD 1.0. The Apache Xerces2 Java parser says: "Xerces2 provides fully conforming XML Schema 1.0 and 1.1 processors."[^6] The Java API names two schema languages in `SchemaFactory`: `XMLConstants.W3C_XML_SCHEMA_NS_URI` (`http://www.w3.org/2001/XMLSchema`) for W3C XML Schema 1.0 and `XMLConstants.RELAXNG_NS_URI` (`http://relaxng.org/ns/structure/1.0`) for RELAX NG 1.0.[^7] A JDK does not have to implement both. `newInstance` throws "IllegalArgumentException - If no implementation of the schema language is available."[^7] The program below asks for both:

```java run id=relaxng-jdk
import javax.xml.XMLConstants;
import javax.xml.validation.SchemaFactory;

public class Main {
    public static void main(String[] args) {
        String[][] languages = {
            {"W3C XML Schema", XMLConstants.W3C_XML_SCHEMA_NS_URI},
            {"RELAX NG", XMLConstants.RELAXNG_NS_URI}};
        for (String[] language : languages) {
            try {
                SchemaFactory.newInstance(language[1]);
                System.out.println(language[0] + ": available");
            } catch (IllegalArgumentException e) {
                System.out.println(language[0] + ": " + e.getClass().getSimpleName());
            }
        }
    }
}
```

```output for=relaxng-jdk
W3C XML Schema: available
RELAX NG: IllegalArgumentException
```

The built-in JDK implementation has no RELAX NG validator. A Java program that uses RELAX NG needs a library such as Jing.

## Pitfalls

- **Validation that changes the data.** An XSD validator, and a DTD parser, can add default attribute values. A program that reads the file without the schema sees different data. Do not put values that change the meaning of a document in schema defaults.
- **A schema that the document chooses.** A `DOCTYPE` declaration or an `xsi:schemaLocation` attribute is part of the input. If the validator loads the schema that the input names, an attacker can choose a weaker schema, or make the parser load a remote file. Load the schema from a trusted place and give it to the validator.
- **Ambiguous content models.** "Unlike SGML, XML and W3C XML Schema, RELAX NG does not restrict content models to be 1-unambiguous".[^1] A RELAX NG pattern can therefore have no direct equivalent in a DTD or in XSD.
- **Unordered content.** In RELAX NG, the `interleave` operator "matches any interleaving of a sequence containing a single A element and a sequence containing zero or more B elements; it thus allows the A element to occur anywhere, including between two B elements."[^1]
- **Identity constraints and scope.** In XSD, the scope of `xs:unique` and `xs:key` is the element that contains the declaration. The design essay of RELAX NG says that this "leads to the restriction that although identity constraints are hierarchical, there is no way to specify a reference to a key in another part of the hierarchy."[^1]
- **Old Schematron.** lxml says that the pre-ISO Schematron namespace `http://www.ascc.net/xml/schematron` is "no longer available in libxml2 2.15".[^8] Use the ISO namespace `http://purl.oclc.org/dsdl/schematron`.
- **XSD 1.0 and 1.1.** The JDK names only W3C XML Schema 1.0 in `SchemaFactory`.[^7] A schema written for XSD 1.1 needs a processor for 1.1, such as Xerces2.[^6]

## Tools and libraries

| Tool | Languages | Notes |
|---|---|---|
| Java `javax.xml.validation` | XSD 1.0, DTD at parse time | `SchemaFactory` and `Validator`; no RELAX NG in the JDK[^7] |
| Apache Xerces2 | XSD 1.0 and 1.1 | Java parser[^6] |
| .NET `System.Xml.Schema` | XSD, DTD | `XmlSchemaSet` "Contains a cache of XML Schema definition language (XSD) schemas."[^9] `XmlSchemaUnique` "can be used to identify a unique constraint among a set of elements."[^10] |
| libxml2 and `xmllint` | DTD, XSD, RELAX NG, Schematron | "Only the XML syntax is supported" for RELAX NG[^2] |
| Python lxml | DTD, RELAX NG, XSD, Schematron | "lxml currently supports three schema languages: DTD , Relax NG and XML Schema ."[^8] |
| Jing | RELAX NG | a Java validator by James Clark; "Supports both XML and compact syntaxes."[^2] |
| Trang | RELAX NG, DTD, XSD | a converter; "W3C XML Schema is supported for output only"[^2] |

lxml adds Schematron on top of libxml2: "lxml also provides support for ISO- Schematron , based on the pure-XSLT skeleton implementation of Schematron".[^8] It can also apply the defaults of a DTD: the parser option `attribute_defaults` "loads the DTD and weaves attribute default values into the document".[^8]

## References
