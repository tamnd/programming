# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/query-xml-with-xpath/.

import pytest

from query_xpath import QueryError, query_xpath

LIBRARY = (
    "<library xmlns:m=\"urn:example:meta\">"
    "<book id=\"b1\" year=\"1999\" m:lang=\"en\"><title>Alpha</title><price>12.50</price></book>"
    "<book id=\"b2\" year=\"2008\"><title>Beta</title><price>30.00</price><note>out of print</note></book>"
    "<shelf>"
    "<book id=\"b3\" year=\"2008\" m:lang=\"fr\"><title>Gamma</title><price>8.25</price></book>"
    "</shelf>"
    "</library>"
)
BOOKS = "<library xmlns=\"urn:example:books\"><book>Alpha</book></library>"
META = {"m": "urn:example:meta"}

# name: (text, path, namespaces, the string values)
CASES = {
    "child-path": (LIBRARY, "book/title", {}, ["Alpha", "Beta"]),
    "descendants": (LIBRARY, ".//title", {}, ["Alpha", "Beta", "Gamma"]),
    "attribute-equals": (LIBRARY, ".//book[@year='2008']/title", {}, ["Beta", "Gamma"]),
    "child-equals": (LIBRARY, "book[title='Beta']/price", {}, ["30.00"]),
    "position": (LIBRARY, "book[2]/title", {}, ["Beta"]),
    "has-attribute": (LIBRARY, ".//book[@m:lang]/title", META, ["Alpha", "Gamma"]),
    "attribute-not-equal": (LIBRARY, ".//book[@m:lang!='en']/title", META, ["Gamma"]),
    "has-child": (LIBRARY, "book[note]/title", {}, ["Beta"]),
    "first-of-each-parent": (LIBRARY, ".//book[1]/title", {}, ["Alpha", "Gamma"]),
    "last-of-each-parent": (LIBRARY, ".//book[last()]/title", {}, ["Beta", "Gamma"]),
    "wildcard": (LIBRARY, "book[1]/*", {}, ["Alpha", "12.50"]),
    "descendant-wildcard": ("<r><a>1<b>2</b></a><c>3</c></r>", ".//*", {}, ["12", "2", "3"]),
    "no-match": (LIBRARY, "book/author", {}, []),
    "self": ("<p>Tom <b>&amp;</b> Jerry</p>", ".", {}, ["Tom & Jerry"]),
    "cdata-and-entities": ("<r><x><![CDATA[a<b]]></x><x>&lt;c&gt;</x></r>", "x", {}, ["a<b", "<c>"]),
    "comment-in-text": ("<r><v>a<!-- note -->b</v></r>", "v", {}, ["ab"]),
    "empty-elements": ("<r><v/><v></v></r>", "v", {}, ["", ""]),
    "default-namespace-unprefixed": (BOOKS, "book", {}, []),
    "default-namespace-prefixed": (BOOKS, "b:book", {"b": "urn:example:books"}, ["Alpha"]),
    "other-prefix": ("<x:list xmlns:x=\"urn:k\"><x:item>1</x:item></x:list>", "k:item", {"k": "urn:k"}, ["1"]),
    "whitespace": ("<r>\n  <v> a </v>\n</r>", "v", {}, [" a "]),
    "non-ascii": ("<r><city>Zürich</city><city>東京</city></r>", "city[.='東京']", {}, ["東京"]),
}

# name: (text, path, namespaces, the kind of error)
INVALID = {
    "not-well-formed": ("<a><b></a>", "b", {}, "parse-error"),
    "empty-text": ("", "b", {}, "parse-error"),
    "unbound-prefix-in-text": ("<x:a/>", "b", {}, "parse-error"),
    "unclosed-predicate": (LIBRARY, "book[@id='b1'", {}, "bad-path"),
    "unbound-prefix-in-path": (LIBRARY, "x:book", {}, "bad-path"),
}


@pytest.mark.parametrize("name", CASES)
def test_cases(name: str) -> None:
    text, path, namespaces, want = CASES[name]
    assert query_xpath(text, path, namespaces) == want


@pytest.mark.parametrize("name", INVALID)
def test_errors(name: str) -> None:
    text, path, namespaces, kind = INVALID[name]
    with pytest.raises(QueryError) as info:
        query_xpath(text, path, namespaces)
    assert info.value.kind == kind
