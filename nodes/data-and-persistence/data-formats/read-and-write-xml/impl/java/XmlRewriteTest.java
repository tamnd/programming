// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-and-write-xml/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class XmlRewriteTest {
    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("element-and-text", "<note><to>Ada</to><body>Hello</body></note>",
                        "<note><to>Ada</to><body>Hello</body></note>"),
                Arguments.of("declaration-and-comment", "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<!-- a list -->\n<list><item>1</item></list>\n",
                        "<list><item>1</item></list>"),
                Arguments.of("empty-element", "<list><item/><item></item></list>",
                        "<list><item></item><item></item></list>"),
                Arguments.of("attributes-sorted", "<book year=\"1999\" id=\"b1\" lang=\"en\"/>",
                        "<book id=\"b1\" lang=\"en\" year=\"1999\"></book>"),
                Arguments.of("escapes-in-text", "<p>Tom &amp; Jerry &lt;3 &gt; 2</p>",
                        "<p>Tom &amp; Jerry &lt;3 &gt; 2</p>"),
                Arguments.of("single-quotes", "<a title='say \"hi\"'/>",
                        "<a title=\"say &quot;hi&quot;\"></a>"),
                Arguments.of("escapes-in-attribute", "<a v=\"x &lt; y &amp; &apos;z&apos;\"/>",
                        "<a v=\"x &lt; y &amp; 'z'\"></a>"),
                Arguments.of("greater-than-in-attribute", "<a v=\"1 &gt; 0\">2 > 1</a>",
                        "<a v=\"1 > 0\">2 &gt; 1</a>"),
                Arguments.of("character-references", "<a v=\"&#9;&#10;&#13;\">&#65;&#x42;&#13;</a>",
                        "<a v=\"&#x9;&#xA;&#xD;\">AB&#xD;</a>"),
                Arguments.of("cdata", "<code><![CDATA[if (a < b && c) {}]]></code>",
                        "<code>if (a &lt; b &amp;&amp; c) {}</code>"),
                Arguments.of("mixed-content", "<p>An <b>old</b> and <i>new</i> text.</p>",
                        "<p>An <b>old</b> and <i>new</i> text.</p>"),
                Arguments.of("whitespace-kept", "<list>\n  <item> a </item>\n</list>",
                        "<list>\n  <item> a </item>\n</list>"),
                Arguments.of("crlf", "<list>\r\n<item>a\r\nb</item>\r\n</list>\r\n",
                        "<list>\n<item>a\nb</item>\n</list>"),
                Arguments.of("attribute-whitespace", "<a v=\"  two  spaces  \"/>",
                        "<a v=\"  two  spaces  \"></a>"),
                Arguments.of("comment-and-pi-inside", "<a>x<!-- note -->y<?app data?>z</a>",
                        "<a>xyz</a>"),
                Arguments.of("non-ascii", "<city name=\"Zürich\">東京 &#x1F600; &#233;</city>",
                        "<city name=\"Zürich\">東京 😀 é</city>"),
                Arguments.of("default-namespace", "<feed xmlns=\"urn:example:feed\"><entry/></feed>",
                        "<feed xmlns=\"urn:example:feed\"><entry></entry></feed>"),
                Arguments.of("prefixes-kept", "<x:list xmlns:x=\"urn:example:x\"><x:item x:id=\"1\"/></x:list>",
                        "<x:list xmlns:x=\"urn:example:x\"><x:item x:id=\"1\"></x:item></x:list>"),
                Arguments.of("declarations-first", "<r b=\"2\" xmlns:z=\"urn:z\" a=\"1\" xmlns=\"urn:d\" xmlns:m=\"urn:m\"/>",
                        "<r xmlns=\"urn:d\" xmlns:m=\"urn:m\" xmlns:z=\"urn:z\" a=\"1\" b=\"2\"></r>"),
                Arguments.of("inner-declaration", "<a xmlns=\"urn:a\"><b xmlns=\"urn:b\"><c/></b></a>",
                        "<a xmlns=\"urn:a\"><b xmlns=\"urn:b\"><c></c></b></a>"));
    }

    static Stream<Arguments> errors() {
        return Stream.of(
                Arguments.of("mismatched-tags", "<a><b></a></b>"),
                Arguments.of("unclosed", "<a><b></b>"),
                Arguments.of("empty-text", ""),
                Arguments.of("two-roots", "<a/><b/>"),
                Arguments.of("text-after-root", "<a/>tail"),
                Arguments.of("bare-ampersand", "<a>Tom & Jerry</a>"),
                Arguments.of("undefined-entity", "<a>&nbsp;</a>"),
                Arguments.of("lt-in-attribute", "<a v=\"1 < 2\"/>"),
                Arguments.of("duplicate-attribute", "<a v=\"1\" v=\"2\"/>"),
                Arguments.of("doctype", "<!DOCTYPE a [<!ENTITY e \"text\">]><a>&e;</a>"),
                Arguments.of("external-entity", "<!DOCTYPE a [<!ENTITY e SYSTEM \"file:///etc/hostname\">]><a>&e;</a>"));
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void rewrites(String name, String text, String want) throws XmlRewrite.XmlException {
        assertEquals(want, XmlRewrite.xmlRewrite(text), name);
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("errors")
    void refuses(String name, String text) {
        XmlRewrite.XmlException e =
                assertThrows(XmlRewrite.XmlException.class, () -> XmlRewrite.xmlRewrite(text), name);
        assertEquals("parse-error", e.kind, name);
    }
}
