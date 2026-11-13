// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-and-write-xml/.

using Xunit;

public sealed class XmlRewriteTests
{
    // case, text, the canonical text
    public static TheoryData<string, string, string> Cases => new()
    {
        { "element-and-text", "<note><to>Ada</to><body>Hello</body></note>", "<note><to>Ada</to><body>Hello</body></note>" },
        { "declaration-and-comment", "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<!-- a list -->\n<list><item>1</item></list>\n", "<list><item>1</item></list>" },
        { "empty-element", "<list><item/><item></item></list>", "<list><item></item><item></item></list>" },
        { "attributes-sorted", "<book year=\"1999\" id=\"b1\" lang=\"en\"/>", "<book id=\"b1\" lang=\"en\" year=\"1999\"></book>" },
        { "escapes-in-text", "<p>Tom &amp; Jerry &lt;3 &gt; 2</p>", "<p>Tom &amp; Jerry &lt;3 &gt; 2</p>" },
        { "single-quotes", "<a title='say \"hi\"'/>", "<a title=\"say &quot;hi&quot;\"></a>" },
        { "escapes-in-attribute", "<a v=\"x &lt; y &amp; &apos;z&apos;\"/>", "<a v=\"x &lt; y &amp; 'z'\"></a>" },
        { "greater-than-in-attribute", "<a v=\"1 &gt; 0\">2 > 1</a>", "<a v=\"1 > 0\">2 &gt; 1</a>" },
        { "character-references", "<a v=\"&#9;&#10;&#13;\">&#65;&#x42;&#13;</a>", "<a v=\"&#x9;&#xA;&#xD;\">AB&#xD;</a>" },
        { "cdata", "<code><![CDATA[if (a < b && c) {}]]></code>", "<code>if (a &lt; b &amp;&amp; c) {}</code>" },
        { "mixed-content", "<p>An <b>old</b> and <i>new</i> text.</p>", "<p>An <b>old</b> and <i>new</i> text.</p>" },
        { "whitespace-kept", "<list>\n  <item> a </item>\n</list>", "<list>\n  <item> a </item>\n</list>" },
        { "crlf", "<list>\r\n<item>a\r\nb</item>\r\n</list>\r\n", "<list>\n<item>a\nb</item>\n</list>" },
        { "attribute-whitespace", "<a v=\"  two  spaces  \"/>", "<a v=\"  two  spaces  \"></a>" },
        { "comment-and-pi-inside", "<a>x<!-- note -->y<?app data?>z</a>", "<a>xyz</a>" },
        { "non-ascii", "<city name=\"Zürich\">東京 &#x1F600; &#233;</city>", "<city name=\"Zürich\">東京 😀 é</city>" },
        { "default-namespace", "<feed xmlns=\"urn:example:feed\"><entry/></feed>", "<feed xmlns=\"urn:example:feed\"><entry></entry></feed>" },
        { "prefixes-kept", "<x:list xmlns:x=\"urn:example:x\"><x:item x:id=\"1\"/></x:list>", "<x:list xmlns:x=\"urn:example:x\"><x:item x:id=\"1\"></x:item></x:list>" },
        { "declarations-first", "<r b=\"2\" xmlns:z=\"urn:z\" a=\"1\" xmlns=\"urn:d\" xmlns:m=\"urn:m\"/>", "<r xmlns=\"urn:d\" xmlns:m=\"urn:m\" xmlns:z=\"urn:z\" a=\"1\" b=\"2\"></r>" },
        { "inner-declaration", "<a xmlns=\"urn:a\"><b xmlns=\"urn:b\"><c/></b></a>", "<a xmlns=\"urn:a\"><b xmlns=\"urn:b\"><c></c></b></a>" },
    };

    // case, text that the function must refuse
    public static TheoryData<string, string> Errors => new()
    {
        { "mismatched-tags", "<a><b></a></b>" },
        { "unclosed", "<a><b></b>" },
        { "empty-text", "" },
        { "two-roots", "<a/><b/>" },
        { "text-after-root", "<a/>tail" },
        { "bare-ampersand", "<a>Tom & Jerry</a>" },
        { "undefined-entity", "<a>&nbsp;</a>" },
        { "lt-in-attribute", "<a v=\"1 < 2\"/>" },
        { "duplicate-attribute", "<a v=\"1\" v=\"2\"/>" },
        { "doctype", "<!DOCTYPE a [<!ENTITY e \"text\">]><a>&e;</a>" },
        { "external-entity", "<!DOCTYPE a [<!ENTITY e SYSTEM \"file:///etc/hostname\">]><a>&e;</a>" },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Rewrites(string name, string text, string want)
    {
        Assert.True(want == XmlRewrite.Rewrite(text), name);
    }

    [Theory]
    [MemberData(nameof(Errors))]
    public void Refuses(string name, string text)
    {
        var e = Assert.Throws<XmlRewriteException>(() => XmlRewrite.Rewrite(text));
        Assert.True(e.Kind == "parse-error", name);
    }
}
