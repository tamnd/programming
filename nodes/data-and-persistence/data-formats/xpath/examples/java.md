```java run id=select-and-evaluate
import java.io.StringReader;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathConstants;
import javax.xml.xpath.XPathFactory;
import org.w3c.dom.Document;
import org.w3c.dom.NodeList;
import org.xml.sax.InputSource;

public class Main {
    static final String TEXT = """
        <library>
          <book id="b1" year="1999"><title>Alpha</title><price>12.50</price></book>
          <book id="b2" year="2008"><title>Beta</title><price>30</price></book>
          <book id="b3" year="2017"><title>Gamma</title><price>8.25</price></book>
        </library>""";

    public static void main(String[] args) throws Exception {
        DocumentBuilderFactory f = DocumentBuilderFactory.newInstance();
        f.setFeature("http://apache.org/xml/features/disallow-doctype-decl", true);
        Document doc = f.newDocumentBuilder().parse(new InputSource(new StringReader(TEXT)));
        XPath xp = XPathFactory.newInstance().newXPath();

        NodeList titles = (NodeList) xp.evaluate("/library/book[@year > 2000]/title", doc, XPathConstants.NODESET);
        for (int i = 0; i < titles.getLength(); i++) {
            System.out.println("title: " + titles.item(i).getTextContent());
        }
        System.out.println("count: " + xp.evaluate("count(//book)", doc));
        System.out.println("sum: " + xp.evaluate("sum(//price)", doc, XPathConstants.NUMBER));
        System.out.println("cheap: " + xp.evaluate("//book[price < 10]/@id", doc));
        System.out.println("first: " + xp.evaluate("//book[1]/title", doc));
        System.out.println("last: " + xp.evaluate("//book[last()]/title", doc));
        System.out.println("parent: " + xp.evaluate("name(//title[. = 'Beta']/..)", doc));
        System.out.println("after: " + xp.evaluate("//book[@id = 'b1']/following-sibling::book[1]/title", doc));

        // A comparison with a node-set is true if it is true for any node.
        System.out.println("= 2008: " + xp.evaluate("//book/@year = 2008", doc));
        System.out.println("!= 2008: " + xp.evaluate("//book/@year != 2008", doc));
        System.out.println("not(= 2008): " + xp.evaluate("not(//book/@year = 2008)", doc));
    }
}
```

```output for=select-and-evaluate
title: Beta
title: Gamma
count: 3
sum: 50.75
cheap: b3
first: Alpha
last: Gamma
parent: book
after: Beta
= 2008: true
!= 2008: true
not(= 2008): false
```

- Verdict: stdlib. The package `javax.xml.xpath` of the JDK implements XPath 1.0 over a DOM tree. `evaluate` with `XPathConstants.NODESET` returns a `NodeList`. With no return type, it returns the string value of the result, so `count(//book)` gives the string `3`. The last three lines show the rule for a comparison with a node-set: `@year = 2008` is true because one book has the year 2008, and `@year != 2008` is also true because another book does not. To say that no book has the year 2008, use `not(@year = 2008)`.
