# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/query-xml-with-xpath/.

import xml.etree.ElementTree as ET


class QueryError(ValueError):
    """The text is not well-formed XML (parse-error), or the path is not valid (bad-path)."""

    def __init__(self, kind: str, detail: str) -> None:
        super().__init__(f"{kind}: {detail}")
        self.kind = kind


def query_xpath(text: str, path: str, namespaces: dict[str, str]) -> list[str]:
    """Returns the string value of each element that the path selects from the root element, in document order."""
    try:
        root = ET.fromstring(text)
    except ET.ParseError as error:
        raise QueryError("parse-error", str(error)) from error
    try:
        # findall supports the subset of XPath 1.0 of the contract. A prefix in the path is looked up in namespaces.
        elements = root.findall(path, namespaces)
    except (SyntaxError, TypeError) as error:
        # ElementTree raises SyntaxError for most bad paths, and TypeError for a predicate with no closing bracket.
        raise QueryError("bad-path", str(error)) from error
    # The string value of an element is all of its descendant text, as in XPath.
    return ["".join(element.itertext()) for element in elements]
