```python run id=expanded-names
import xml.etree.ElementTree as ET

text = """<catalog xmlns="urn:example:books" xmlns:m="urn:example:meta">
  <book id="b1" m:lang="en">
    <title>Tide tables</title>
    <price xmlns="urn:example:money" currency="EUR">32.18</price>
  </book>
</catalog>"""
root = ET.fromstring(text)
book = root[0]
print(root.tag)
print(book.attrib)
for child in book:
    print(child.tag, child.attrib)

ns = {"b": "urn:example:books", "money": "urn:example:money"}
print(root.find("b:book/b:title", ns).text)
print(root.find("b:book/money:price", ns).text)
print(root.find("book"))
print(ET.tostring(book[0], encoding="unicode").strip())
```

```output for=expanded-names
{urn:example:books}catalog
{'id': 'b1', '{urn:example:meta}lang': 'en'}
{urn:example:books}title {}
{urn:example:money}price {'currency': 'EUR'}
Tide tables
32.18
None
<ns0:title xmlns:ns0="urn:example:books">Tide tables</ns0:title>
```

- Verdict: stdlib. ElementTree writes each expanded name in the Clark notation `{namespace}local`, and it drops the prefixes of the text. `find` takes a map from the prefixes of the query to the namespace names, and these prefixes do not need to match the prefixes of the document. A name with no namespace does not match an element in a namespace, so `find("book")` returns `None`. On output, ElementTree makes new prefixes `ns0`, `ns1` and so on, unless the program calls `register_namespace`.
