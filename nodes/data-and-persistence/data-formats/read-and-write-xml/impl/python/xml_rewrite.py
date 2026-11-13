# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/read-and-write-xml/.

from xml.dom import minidom
from xml.parsers.expat import ExpatError


class XmlError(ValueError):
    """The text is not well-formed XML, or it has a document type declaration (parse-error)."""

    def __init__(self, kind: str, detail: str) -> None:
        super().__init__(f"{kind}: {detail}")
        self.kind = kind


def escape_text(text: str) -> str:
    # & first, so that the other escapes are not escaped again.
    return text.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;").replace("\r", "&#xD;")


def escape_attribute(value: str) -> str:
    value = value.replace("&", "&amp;").replace("<", "&lt;").replace('"', "&quot;")
    # A parser turns a literal tab, line feed or carriage return in a value into a space, so these must be references.
    return value.replace("\t", "&#x9;").replace("\n", "&#xA;").replace("\r", "&#xD;")


def attribute_key(name: str) -> tuple[int, str]:
    # The default namespace declaration comes first, then the other declarations, then the attributes.
    if name == "xmlns":
        return (0, "")
    if name.startswith("xmlns:"):
        return (1, name)
    return (2, name)


def write(node: minidom.Node, out: list[str]) -> None:
    if node.nodeType in (node.TEXT_NODE, node.CDATA_SECTION_NODE):
        out.append(escape_text(node.data))
    elif node.nodeType == node.ELEMENT_NODE:
        out.append("<" + node.tagName)
        for name in sorted(node.attributes.keys(), key=attribute_key):
            out.append(f' {name}="{escape_attribute(node.getAttribute(name))}"')
        out.append(">")
        for child in node.childNodes:
            write(child, out)
        out.append(f"</{node.tagName}>")
    # Comments and processing instructions are not written.


def xml_rewrite(text: str) -> str:
    """Parses the XML text to a tree and writes it back in the canonical form of the contract."""
    try:
        doc = minidom.parseString(text)
    except ExpatError as error:
        raise XmlError("parse-error", str(error)) from error
    # Expat does not load external entities, but it does expand the internal ones. The contract refuses both.
    if doc.doctype is not None:
        raise XmlError("parse-error", "a document type declaration is not allowed")
    out: list[str] = []
    write(doc.documentElement, out)
    return "".join(out)
