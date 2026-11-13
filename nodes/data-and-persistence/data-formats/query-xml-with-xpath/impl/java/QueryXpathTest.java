// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/query-xml-with-xpath/.

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.util.List;
import java.util.Map;
import java.util.stream.Stream;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

class QueryXpathTest {
    static final String LIBRARY =
            "<library xmlns:m=\"urn:example:meta\">"
            + "<book id=\"b1\" year=\"1999\" m:lang=\"en\"><title>Alpha</title><price>12.50</price></book>"
            + "<book id=\"b2\" year=\"2008\"><title>Beta</title><price>30.00</price><note>out of print</note></book>"
            + "<shelf>"
            + "<book id=\"b3\" year=\"2008\" m:lang=\"fr\"><title>Gamma</title><price>8.25</price></book>"
            + "</shelf>"
            + "</library>";
    static final String BOOKS = "<library xmlns=\"urn:example:books\"><book>Alpha</book></library>";
    static final Map<String, String> META = Map.of("m", "urn:example:meta");

    static Stream<Arguments> cases() {
        return Stream.of(
                Arguments.of("child-path", LIBRARY, "book/title", Map.of(),
                        List.of("Alpha", "Beta")),
                Arguments.of("descendants", LIBRARY, ".//title", Map.of(),
                        List.of("Alpha", "Beta", "Gamma")),
                Arguments.of("attribute-equals", LIBRARY, ".//book[@year='2008']/title", Map.of(),
                        List.of("Beta", "Gamma")),
                Arguments.of("child-equals", LIBRARY, "book[title='Beta']/price", Map.of(),
                        List.of("30.00")),
                Arguments.of("position", LIBRARY, "book[2]/title", Map.of(),
                        List.of("Beta")),
                Arguments.of("has-attribute", LIBRARY, ".//book[@m:lang]/title", META,
                        List.of("Alpha", "Gamma")),
                Arguments.of("attribute-not-equal", LIBRARY, ".//book[@m:lang!='en']/title", META,
                        List.of("Gamma")),
                Arguments.of("has-child", LIBRARY, "book[note]/title", Map.of(),
                        List.of("Beta")),
                Arguments.of("first-of-each-parent", LIBRARY, ".//book[1]/title", Map.of(),
                        List.of("Alpha", "Gamma")),
                Arguments.of("last-of-each-parent", LIBRARY, ".//book[last()]/title", Map.of(),
                        List.of("Beta", "Gamma")),
                Arguments.of("wildcard", LIBRARY, "book[1]/*", Map.of(),
                        List.of("Alpha", "12.50")),
                Arguments.of("descendant-wildcard", "<r><a>1<b>2</b></a><c>3</c></r>", ".//*", Map.of(),
                        List.of("12", "2", "3")),
                Arguments.of("no-match", LIBRARY, "book/author", Map.of(),
                        List.of()),
                Arguments.of("self", "<p>Tom <b>&amp;</b> Jerry</p>", ".", Map.of(),
                        List.of("Tom & Jerry")),
                Arguments.of("cdata-and-entities", "<r><x><![CDATA[a<b]]></x><x>&lt;c&gt;</x></r>", "x", Map.of(),
                        List.of("a<b", "<c>")),
                Arguments.of("comment-in-text", "<r><v>a<!-- note -->b</v></r>", "v", Map.of(),
                        List.of("ab")),
                Arguments.of("empty-elements", "<r><v/><v></v></r>", "v", Map.of(),
                        List.of("", "")),
                Arguments.of("default-namespace-unprefixed", BOOKS, "book", Map.of(),
                        List.of()),
                Arguments.of("default-namespace-prefixed", BOOKS, "b:book", Map.of("b", "urn:example:books"),
                        List.of("Alpha")),
                Arguments.of("other-prefix", "<x:list xmlns:x=\"urn:k\"><x:item>1</x:item></x:list>", "k:item", Map.of("k", "urn:k"),
                        List.of("1")),
                Arguments.of("whitespace", "<r>\n  <v> a </v>\n</r>", "v", Map.of(),
                        List.of(" a ")),
                Arguments.of("non-ascii", "<r><city>Zürich</city><city>東京</city></r>", "city[.='東京']", Map.of(),
                        List.of("東京")));
    }

    static Stream<Arguments> errors() {
        return Stream.of(
                Arguments.of("not-well-formed", "<a><b></a>", "b", Map.of(), "parse-error"),
                Arguments.of("empty-text", "", "b", Map.of(), "parse-error"),
                Arguments.of("unbound-prefix-in-text", "<x:a/>", "b", Map.of(), "parse-error"),
                Arguments.of("unclosed-predicate", LIBRARY, "book[@id='b1'", Map.of(), "bad-path"),
                Arguments.of("unbound-prefix-in-path", LIBRARY, "x:book", Map.of(), "bad-path"));
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("cases")
    void selects(String name, String text, String path, Map<String, String> namespaces, List<String> want)
            throws QueryXpath.QueryException {
        assertEquals(want, QueryXpath.queryXpath(text, path, namespaces), name);
    }

    @ParameterizedTest(name = "{0}")
    @MethodSource("errors")
    void reports(String name, String text, String path, Map<String, String> namespaces, String kind) {
        QueryXpath.QueryException e = assertThrows(
                QueryXpath.QueryException.class, () -> QueryXpath.queryXpath(text, path, namespaces), name);
        assertEquals(kind, e.kind, name);
    }
}
