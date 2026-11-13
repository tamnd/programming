```python run id=parse-and-write
import xml.etree.ElementTree as ET

text = """<?xml version="1.0" encoding="UTF-8"?>
<!-- a library with one book -->
<library city="Zürich">
  <book id="b1" lang="en">
    <title>Tom &amp; Jerry &lt;3</title>
    <note><![CDATA[a < b && b > c]]></note>
  </book>
</library>
"""
root = ET.fromstring(text)
print(root.tag, root.attrib)
book = root.find("book")
print(book.get("id"), book.get("lang"))
print(book.find("title").text)
print(book.find("note").text)
print(repr(root.text))

out = ET.Element("book", {"id": "b2"})
ET.SubElement(out, "title").text = "Q&A <draft>"
print(ET.tostring(out, encoding="unicode"))
```

```output for=parse-and-write
library {'city': 'Zürich'}
b1 en
Tom & Jerry <3
a < b && b > c
'\n  '
<book id="b2"><title>Q&amp;A &lt;draft&gt;</title></book>
```

- Verdict: stdlib. `xml.etree.ElementTree` parses the text into a tree of `Element` objects. The parser drops the XML declaration and the comment by default, resolves the entity references and returns the CDATA section as plain text. The whitespace between the tags stays in the tree as the `text` of the parent element. `tostring` escapes `&`, `<` and `>` in text. The parser is Expat, which does not load external entities.
