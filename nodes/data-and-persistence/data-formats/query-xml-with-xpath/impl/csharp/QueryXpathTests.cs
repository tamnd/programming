// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/query-xml-with-xpath/.

using Xunit;

public sealed class QueryXpathTests
{
    private const string Library =
        "<library xmlns:m=\"urn:example:meta\">"
        + "<book id=\"b1\" year=\"1999\" m:lang=\"en\"><title>Alpha</title><price>12.50</price></book>"
        + "<book id=\"b2\" year=\"2008\"><title>Beta</title><price>30.00</price><note>out of print</note></book>"
        + "<shelf>"
        + "<book id=\"b3\" year=\"2008\" m:lang=\"fr\"><title>Gamma</title><price>8.25</price></book>"
        + "</shelf>"
        + "</library>";
    private const string Books = "<library xmlns=\"urn:example:books\"><book>Alpha</book></library>";

    // The namespaces are "prefix=uri" pairs joined by "|", so that TheoryData can hold them.
    private static Dictionary<string, string> Namespaces(string pairs) =>
        pairs.Split('|', StringSplitOptions.RemoveEmptyEntries)
            .Select(pair => pair.Split('=', 2))
            .ToDictionary(pair => pair[0], pair => pair[1]);

    // case, text, path, namespaces, the string values
    public static TheoryData<string, string, string, string, string[]> Cases => new()
    {
        { "child-path", Library, "book/title", "", ["Alpha", "Beta"] },
        { "descendants", Library, ".//title", "", ["Alpha", "Beta", "Gamma"] },
        { "attribute-equals", Library, ".//book[@year='2008']/title", "", ["Beta", "Gamma"] },
        { "child-equals", Library, "book[title='Beta']/price", "", ["30.00"] },
        { "position", Library, "book[2]/title", "", ["Beta"] },
        { "has-attribute", Library, ".//book[@m:lang]/title", "m=urn:example:meta", ["Alpha", "Gamma"] },
        { "attribute-not-equal", Library, ".//book[@m:lang!='en']/title", "m=urn:example:meta", ["Gamma"] },
        { "has-child", Library, "book[note]/title", "", ["Beta"] },
        { "first-of-each-parent", Library, ".//book[1]/title", "", ["Alpha", "Gamma"] },
        { "last-of-each-parent", Library, ".//book[last()]/title", "", ["Beta", "Gamma"] },
        { "wildcard", Library, "book[1]/*", "", ["Alpha", "12.50"] },
        { "descendant-wildcard", "<r><a>1<b>2</b></a><c>3</c></r>", ".//*", "", ["12", "2", "3"] },
        { "no-match", Library, "book/author", "", [] },
        { "self", "<p>Tom <b>&amp;</b> Jerry</p>", ".", "", ["Tom & Jerry"] },
        { "cdata-and-entities", "<r><x><![CDATA[a<b]]></x><x>&lt;c&gt;</x></r>", "x", "", ["a<b", "<c>"] },
        { "comment-in-text", "<r><v>a<!-- note -->b</v></r>", "v", "", ["ab"] },
        { "empty-elements", "<r><v/><v></v></r>", "v", "", ["", ""] },
        { "default-namespace-unprefixed", Books, "book", "", [] },
        { "default-namespace-prefixed", Books, "b:book", "b=urn:example:books", ["Alpha"] },
        { "other-prefix", "<x:list xmlns:x=\"urn:k\"><x:item>1</x:item></x:list>", "k:item", "k=urn:k", ["1"] },
        { "whitespace", "<r>\n  <v> a </v>\n</r>", "v", "", [" a "] },
        { "non-ascii", "<r><city>Zürich</city><city>東京</city></r>", "city[.='東京']", "", ["東京"] },
    };

    // case, text, path, namespaces, the kind of error
    public static TheoryData<string, string, string, string, string> Errors => new()
    {
        { "not-well-formed", "<a><b></a>", "b", "", "parse-error" },
        { "empty-text", "", "b", "", "parse-error" },
        { "unbound-prefix-in-text", "<x:a/>", "b", "", "parse-error" },
        { "unclosed-predicate", Library, "book[@id='b1'", "", "bad-path" },
        { "unbound-prefix-in-path", Library, "x:book", "", "bad-path" },
    };

    [Theory]
    [MemberData(nameof(Cases))]
    public void Selects(string name, string text, string path, string namespaces, string[] want)
    {
        Assert.True(want.SequenceEqual(QueryXpath.Query(text, path, Namespaces(namespaces))), name);
    }

    [Theory]
    [MemberData(nameof(Errors))]
    public void Reports(string name, string text, string path, string namespaces, string kind)
    {
        var e = Assert.Throws<QueryException>(() => QueryXpath.Query(text, path, Namespaces(namespaces)));
        Assert.True(kind == e.Kind, name);
    }
}
