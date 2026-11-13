```java run id=parse-and-write
import java.io.StringReader;
import java.io.StringWriter;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.xml.sax.InputSource;
import org.xml.sax.SAXParseException;

public class Main {
    public static void main(String[] args) throws Exception {
        DocumentBuilderFactory f = DocumentBuilderFactory.newInstance();
        f.setNamespaceAware(true);
        f.setFeature("http://apache.org/xml/features/disallow-doctype-decl", true);
        DocumentBuilder b = f.newDocumentBuilder();

        String text = "<library city=\"Zürich\"><book id=\"b1\"><title>Tom &amp; Jerry</title></book></library>";
        Document doc = b.parse(new InputSource(new StringReader(text)));
        Element root = doc.getDocumentElement();
        Element book = (Element) root.getElementsByTagName("book").item(0);
        System.out.println(root.getTagName() + " " + root.getAttribute("city"));
        System.out.println(book.getAttribute("id") + " " + book.getElementsByTagName("title").item(0).getTextContent());

        Document out = b.newDocument();
        Element e = out.createElement("book");
        e.setAttribute("id", "b2");
        Element t = out.createElement("title");
        t.setTextContent("Q&A <draft>");
        e.appendChild(t);
        out.appendChild(e);
        Transformer tr = TransformerFactory.newInstance().newTransformer();
        tr.setOutputProperty(OutputKeys.OMIT_XML_DECLARATION, "yes");
        StringWriter w = new StringWriter();
        tr.transform(new DOMSource(out), new StreamResult(w));
        System.out.println(w);

        try {
            b.parse(new InputSource(new StringReader("<a>Tom & Jerry</a>")));
        } catch (SAXParseException ex) {
            System.out.println("SAXParseException at line " + ex.getLineNumber() + ", column " + ex.getColumnNumber());
        }
    }
}
```

```output for=parse-and-write
library Zürich
b1 Tom & Jerry
<book id="b2"><title>Q&amp;A &lt;draft&gt;</title></book>
SAXParseException at line 1, column 9
```

- Verdict: stdlib. The JAXP API in `javax.xml.parsers` parses the text to a DOM `Document`, and a `Transformer` with no style sheet writes a DOM tree back as text, with `&` and `<` escaped. The feature `disallow-doctype-decl` makes the parser refuse any document type declaration, which blocks the entity attacks that need a DTD. The parser also writes each fatal error to the standard error stream before it raises the exception. A text that is not well-formed raises `SAXParseException` with the line and the column.
