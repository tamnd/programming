// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/query-xml-with-xpath/.

import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import javax.xml.XMLConstants;
import javax.xml.namespace.NamespaceContext;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathConstants;
import javax.xml.xpath.XPathExpressionException;
import javax.xml.xpath.XPathFactory;
import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

public final class QueryXpath {
    /** The text is not well-formed XML (parse-error), or the path is not valid (bad-path). */
    public static final class QueryException extends Exception {
        private static final long serialVersionUID = 1L;
        public final String kind;

        QueryException(String kind, Throwable cause) {
            super(kind + ": " + cause.getMessage(), cause);
            this.kind = kind;
        }
    }

    private QueryXpath() {}

    /** Returns the string value of each element that the path selects from the root element, in document order. */
    public static List<String> queryXpath(String text, String path, Map<String, String> namespaces)
            throws QueryException {
        Document doc;
        try {
            DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
            // Without this, the parser gives no namespace to any element, and a prefix in the path never matches.
            factory.setNamespaceAware(true);
            // A document type declaration can define entities that read files; the contract has none, so refuse it.
            factory.setFeature("http://apache.org/xml/features/disallow-doctype-decl", true);
            DocumentBuilder builder = factory.newDocumentBuilder();
            // The default error handler prints each fatal error to System.err; DefaultHandler only throws it.
            builder.setErrorHandler(new DefaultHandler());
            doc = builder.parse(new InputSource(new StringReader(text)));
        } catch (SAXException | IOException e) {
            throw new QueryException("parse-error", e);
        } catch (ParserConfigurationException e) {
            throw new IllegalStateException(e);
        }
        XPath xpath = XPathFactory.newInstance().newXPath();
        xpath.setNamespaceContext(new NamespaceContext() {
            @Override
            public String getNamespaceURI(String prefix) {
                // An unknown prefix maps to no namespace, and the evaluation then fails.
                return namespaces.getOrDefault(prefix, XMLConstants.NULL_NS_URI);
            }

            @Override
            public String getPrefix(String uri) {
                throw new UnsupportedOperationException();
            }

            @Override
            public Iterator<String> getPrefixes(String uri) {
                throw new UnsupportedOperationException();
            }
        });
        NodeList nodes;
        try {
            nodes = (NodeList) xpath.evaluate(path, doc.getDocumentElement(), XPathConstants.NODESET);
        } catch (XPathExpressionException e) {
            throw new QueryException("bad-path", e);
        }
        List<String> values = new ArrayList<>();
        for (int i = 0; i < nodes.getLength(); i++) {
            Node node = nodes.item(i);
            // getTextContent joins the text of all descendants and skips comments, as the XPath string value does.
            values.add(node.getTextContent());
        }
        return values;
    }
}
