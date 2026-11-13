// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-and-write-xml/.

import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import javax.xml.XMLConstants;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import org.w3c.dom.Attr;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

public final class XmlRewrite {
    /** The text is not well-formed XML, or it has a document type declaration (parse-error). */
    public static final class XmlException extends Exception {
        private static final long serialVersionUID = 1L;
        public final String kind;

        XmlException(String kind, String message, Throwable cause) {
            super(kind + ": " + message, cause);
            this.kind = kind;
        }
    }

    private XmlRewrite() {}

    /** Parses the XML text to a tree and writes it back in the canonical form of the contract. */
    public static String xmlRewrite(String text) throws XmlException {
        Document doc;
        try {
            DocumentBuilderFactory factory = DocumentBuilderFactory.newDefaultInstance();
            factory.setNamespaceAware(true);
            // With this feature the parser stops at <!DOCTYPE, before it reads any entity declaration.
            factory.setFeature("http://apache.org/xml/features/disallow-doctype-decl", true);
            factory.setFeature(XMLConstants.FEATURE_SECURE_PROCESSING, true);
            DocumentBuilder builder = factory.newDocumentBuilder();
            // The default error handler prints each fatal error to System.err; this one only throws it.
            builder.setErrorHandler(new DefaultHandler());
            doc = builder.parse(new InputSource(new StringReader(text)));
        } catch (SAXException | IOException e) {
            throw new XmlException("parse-error", e.getMessage(), e);
        } catch (ParserConfigurationException e) {
            throw new IllegalStateException(e);
        }
        StringBuilder out = new StringBuilder();
        write(doc.getDocumentElement(), out);
        return out.toString();
    }

    private static void write(Node node, StringBuilder out) {
        switch (node.getNodeType()) {
            // A CDATA section is a Text node too, and it is written as escaped text.
            case Node.TEXT_NODE, Node.CDATA_SECTION_NODE -> escapeText(node.getNodeValue(), out);
            case Node.ELEMENT_NODE -> {
                Element element = (Element) node;
                out.append('<').append(element.getTagName());
                NamedNodeMap map = element.getAttributes();
                List<Attr> attributes = new ArrayList<>();
                for (int i = 0; i < map.getLength(); i++) {
                    attributes.add((Attr) map.item(i));
                }
                attributes.sort(Comparator.comparing(XmlRewrite::attributeKey));
                for (Attr attribute : attributes) {
                    out.append(' ').append(attribute.getName()).append("=\"");
                    escapeAttribute(attribute.getValue(), out);
                    out.append('"');
                }
                out.append('>');
                NodeList children = element.getChildNodes();
                for (int i = 0; i < children.getLength(); i++) {
                    write(children.item(i), out);
                }
                out.append("</").append(element.getTagName()).append('>');
            }
            default -> {
                // Comments and processing instructions are not written.
            }
        }
    }

    private static String attributeKey(Attr attribute) {
        // The default namespace declaration comes first, then the other declarations, then the attributes.
        String name = attribute.getName();
        if (name.equals("xmlns")) {
            return "0";
        }
        return (name.startsWith("xmlns:") ? "1" : "2") + name;
    }

    private static void escapeText(String text, StringBuilder out) {
        for (int i = 0; i < text.length(); i++) {
            char c = text.charAt(i);
            switch (c) {
                case '&' -> out.append("&amp;");
                case '<' -> out.append("&lt;");
                case '>' -> out.append("&gt;");
                case '\r' -> out.append("&#xD;");
                default -> out.append(c);
            }
        }
    }

    private static void escapeAttribute(String value, StringBuilder out) {
        for (int i = 0; i < value.length(); i++) {
            char c = value.charAt(i);
            switch (c) {
                case '&' -> out.append("&amp;");
                case '<' -> out.append("&lt;");
                case '"' -> out.append("&quot;");
                // A parser turns a literal tab, line feed or carriage return in a value into a space.
                case '\t' -> out.append("&#x9;");
                case '\n' -> out.append("&#xA;");
                case '\r' -> out.append("&#xD;");
                default -> out.append(c);
            }
        }
    }
}
