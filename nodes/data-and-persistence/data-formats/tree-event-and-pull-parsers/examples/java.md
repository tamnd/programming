```java run id=three-models
import java.io.StringReader;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.SAXParserFactory;
import javax.xml.stream.XMLInputFactory;
import javax.xml.stream.XMLStreamConstants;
import javax.xml.stream.XMLStreamReader;
import org.w3c.dom.Document;
import org.w3c.dom.NodeList;
import org.xml.sax.Attributes;
import org.xml.sax.InputSource;
import org.xml.sax.helpers.DefaultHandler;

public class Main {
    static final String TEXT = """
        <orders>
          <order id="1"><total>9.50</total></order>
          <order id="2"><total>20.00</total></order>
          <order id="3"><total>4.25</total></order>
        </orders>""";

    public static void main(String[] args) throws Exception {
        // Tree: DOM.
        DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
        dbf.setFeature("http://apache.org/xml/features/disallow-doctype-decl", true);
        Document doc = dbf.newDocumentBuilder().parse(new InputSource(new StringReader(TEXT)));
        NodeList totals = doc.getElementsByTagName("total");
        double sum = 0;
        for (int i = 0; i < totals.getLength(); i++) {
            sum += Double.parseDouble(totals.item(i).getTextContent());
        }
        System.out.println("tree (DOM): " + sum);

        // Event (push): SAX calls the handler.
        SAXParserFactory spf = SAXParserFactory.newInstance();
        spf.setFeature("http://apache.org/xml/features/disallow-doctype-decl", true);
        double[] pushed = {0};
        spf.newSAXParser().parse(new InputSource(new StringReader(TEXT)), new DefaultHandler() {
            StringBuilder text;

            @Override
            public void startElement(String uri, String local, String qName, Attributes atts) {
                if (qName.equals("total")) text = new StringBuilder();
            }

            @Override
            public void characters(char[] ch, int start, int length) {
                if (text != null) text.append(ch, start, length);
            }

            @Override
            public void endElement(String uri, String local, String qName) {
                if (qName.equals("total")) {
                    pushed[0] += Double.parseDouble(text.toString());
                    text = null;
                }
            }
        });
        System.out.println("event (SAX): " + pushed[0]);

        // Pull: StAX, the program calls next().
        XMLInputFactory xif = XMLInputFactory.newInstance();
        xif.setProperty(XMLInputFactory.SUPPORT_DTD, false);
        XMLStreamReader r = xif.createXMLStreamReader(new StringReader(TEXT));
        double pulled = 0;
        while (r.hasNext()) {
            if (r.next() == XMLStreamConstants.START_ELEMENT && r.getLocalName().equals("total")) {
                pulled += Double.parseDouble(r.getElementText());
            }
        }
        System.out.println("pull (StAX): " + pulled);
    }
}
```

```output for=three-models
tree (DOM): 33.75
event (SAX): 33.75
pull (StAX): 33.75
```

- Verdict: stdlib. The JDK has all three models in the module `java.xml`. `DocumentBuilder` builds a DOM tree. `SAXParser` pushes the events to a `DefaultHandler`, and the handler collects the text of `total` because `characters` can come in pieces. `XMLStreamReader` of StAX is a pull parser: the loop calls `next`, and `getElementText` reads the text of the current element. Each factory turns off DTDs, for the reasons on the XML page.
