```python run id=element-path
import xml.etree.ElementTree as ET

text = """<library>
  <book id="b1" year="1999"><title>Alpha</title><price>12.50</price></book>
  <book id="b2" year="2008"><title>Beta</title><price>30</price></book>
  <book id="b3" year="2017"><title>Gamma</title><price>8.25</price></book>
</library>"""
root = ET.fromstring(text)

print([t.text for t in root.findall("./book/title")])
print(root.find(".//book[@id='b2']/title").text)
print(root.find("./book[last()]/title").text)
print([b.get("id") for b in root.findall("./book[title='Gamma']")])
print(root.find(".//title[.='Alpha']/..").get("year"))

try:
    root.findall("./book[@year > 2000]")
except SyntaxError as error:
    print("SyntaxError:", error)
```

```output for=element-path
['Alpha', 'Beta', 'Gamma']
Beta
Gamma
['b3']
1999
SyntaxError: invalid predicate
```

- Verdict: stdlib, a subset. `ElementTree` supports a small part of the abbreviated syntax in `find` and `findall`: child steps, `//`, `..`, `*`, `[@attr]`, `[@attr=value]`, `[tag]`, `[tag=text]`, `[.=text]`, the same tests with `!=`, a position and `last()`. It has no functions, no arithmetic and no comparisons other than `=` and `!=`, so the predicate `[@year > 2000]` raises a `SyntaxError`. A path must be relative to the element, so it starts with `.` or a name. For full XPath 1.0, use the library lxml.
