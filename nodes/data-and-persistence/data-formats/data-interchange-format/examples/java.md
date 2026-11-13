```java run id=binary-layout-and-xml
import java.io.*;
import java.nio.charset.StandardCharsets;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import org.w3c.dom.Document;
import org.w3c.dom.Element;

public class Main {
    public static void main(String[] args) throws Exception {
        int id = 42;
        String name = "Ada";
        boolean active = true;

        // Binary: the bytes have no names and no types. The reader must know the layout.
        ByteArrayOutputStream bytes = new ByteArrayOutputStream();
        try (DataOutputStream out = new DataOutputStream(bytes)) {
            out.writeInt(id);
            out.writeUTF(name);
            out.writeBoolean(active);
        }
        byte[] bin = bytes.toByteArray();
        StringBuilder hex = new StringBuilder();
        for (byte b : bin) hex.append(String.format("%02x ", b));
        System.out.println(bin.length + " bytes: " + hex.toString().trim());

        try (DataInputStream in = new DataInputStream(new ByteArrayInputStream(bin))) {
            System.out.println(in.readInt() + " " + in.readUTF() + " " + in.readBoolean());
        }
        // The same bytes read with a different layout give a wrong value and no error.
        try (DataInputStream in = new DataInputStream(new ByteArrayInputStream(bin))) {
            System.out.println("read as a long: " + in.readLong());
        }

        // Text: XML with the JDK's own DOM and transformer.
        Document doc = DocumentBuilderFactory.newInstance().newDocumentBuilder().newDocument();
        Element user = doc.createElement("user");
        user.setAttribute("id", Integer.toString(id));
        user.setAttribute("active", Boolean.toString(active));
        user.appendChild(doc.createElement("name")).setTextContent(name);
        doc.appendChild(user);

        var t = TransformerFactory.newInstance().newTransformer();
        t.setOutputProperty(OutputKeys.OMIT_XML_DECLARATION, "yes");
        StringWriter sw = new StringWriter();
        t.transform(new DOMSource(doc), new StreamResult(sw));
        String xml = sw.toString();
        System.out.println(xml.getBytes(StandardCharsets.UTF_8).length + " bytes: " + xml);

        Document back = DocumentBuilderFactory.newInstance().newDocumentBuilder()
            .parse(new ByteArrayInputStream(xml.getBytes(StandardCharsets.UTF_8)));
        Element root = back.getDocumentElement();
        System.out.println(root.getAttribute("id") + " " + root.getElementsByTagName("name").item(0).getTextContent());
    }
}
```

```output for=binary-layout-and-xml
10 bytes: 00 00 00 2a 00 03 41 64 61 01
42 Ada true
read as a long: 180388839780
51 bytes: <user active="true" id="42"><name>Ada</name></user>
42 Ada
```

- Verdict: stdlib. Java SE has XML in the module `java.xml` and binary streams in `java.io`, but no JSON API. `DataOutputStream` writes an `int` as four big-endian bytes and a string with `writeUTF` as a two-byte length and the bytes, so the record takes 10 bytes with no names. When a reader uses the wrong layout, `readLong` takes the first eight bytes and gives a wrong number with no error. The DOM and the `Transformer` of the JDK write the XML, and the output has the attributes in name order, not in the order of the code.
