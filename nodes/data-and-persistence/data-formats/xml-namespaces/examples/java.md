```java run id=expanded-names
import java.io.StringReader;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;
import org.xml.sax.InputSource;

public class Main {
    static final String TEXT = """
        <catalog xmlns="urn:example:books" xmlns:m="urn:example:meta">
          <book id="b1" m:lang="en">
            <title>Tide tables</title>
            <price xmlns="urn:example:money" currency="EUR">32.18</price>
          </book>
        </catalog>""";

    static Document parse(boolean aware) throws Exception {
        DocumentBuilderFactory f = DocumentBuilderFactory.newInstance();
        f.setNamespaceAware(aware);
        f.setFeature("http://apache.org/xml/features/disallow-doctype-decl", true);
        DocumentBuilder b = f.newDocumentBuilder();
        return b.parse(new InputSource(new StringReader(TEXT)));
    }

    public static void main(String[] args) throws Exception {
        Document doc = parse(true);
        NodeList all = doc.getElementsByTagNameNS("*", "*");
        for (int i = 0; i < all.getLength(); i++) {
            Element e = (Element) all.item(i);
            System.out.println(e.getNamespaceURI() + " | " + e.getLocalName() + " | " + e.getTagName());
        }
        Element book = (Element) doc.getElementsByTagNameNS("urn:example:books", "book").item(0);
        System.out.println(book.getAttributeNS("urn:example:meta", "lang"));
        System.out.println(doc.getElementsByTagNameNS("urn:example:books", "price").getLength());

        Document plain = parse(false);
        Element first = (Element) plain.getElementsByTagName("book").item(0);
        System.out.println(first.getNamespaceURI() + " | " + first.getLocalName() + " | " + first.getAttribute("m:lang"));
    }
}
```

```output for=expanded-names
urn:example:books | catalog | catalog
urn:example:books | book | book
urn:example:books | title | title
urn:example:money | price | price
en
0
null | null | en
```

- Verdict: stdlib. A JAXP `DocumentBuilderFactory` is not namespace-aware by default, so the program calls `setNamespaceAware(true)`. Then `getNamespaceURI` and `getLocalName` give the two parts of the expanded name, and `getElementsByTagNameNS` and `getAttributeNS` search by them. The `price` element is in the namespace `urn:example:money`, so a search in the books namespace does not find it. With a parser that is not namespace-aware, the namespace and the local name are `null`, and the program must use the qualified name `m:lang` as a plain string.
