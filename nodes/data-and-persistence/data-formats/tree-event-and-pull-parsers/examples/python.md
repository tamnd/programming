```python run id=three-models
import io
import xml.sax
import xml.etree.ElementTree as ET

text = b"""<orders>
  <order id="1"><total>9.50</total></order>
  <order id="2"><total>20.00</total></order>
  <order id="3"><total>4.25</total></order>
</orders>"""

# Tree: parse the whole document, then walk it.
root = ET.fromstring(text)
print("tree:", sum(float(o.findtext("total")) for o in root.iter("order")))


# Event (push): the parser calls the handler for each start tag, text and end tag.
class Totals(xml.sax.ContentHandler):
    def __init__(self):
        super().__init__()
        self.sum, self.inside, self.buffer = 0.0, False, []

    def startElement(self, name, attrs):
        if name == "total":
            self.inside, self.buffer = True, []

    def characters(self, content):
        if self.inside:
            self.buffer.append(content)

    def endElement(self, name):
        if name == "total":
            self.sum += float("".join(self.buffer))
            self.inside = False


handler = Totals()
xml.sax.parseString(text, handler)
print("event:", handler.sum)

# Pull: the program asks for the next event, and clears each order after use.
total = 0.0
for event, element in ET.iterparse(io.BytesIO(text), events=("end",)):
    if element.tag == "order":
        total += float(element.findtext("total"))
        element.clear()
print("pull:", total)
```

```output for=three-models
tree: 33.75
event: 33.75
pull: 33.75
```

- Verdict: stdlib. `ElementTree.fromstring` builds the tree. `xml.sax` is the event parser: the program gives a handler, and the parser calls `startElement`, `characters` and `endElement`. The handler must keep its own state, here a flag and a buffer, because `characters` can come in more than one piece. `iterparse` is a pull parser that also builds the elements: the program reads the events in a loop and clears each `order` after it uses it. The cleared elements stay in the tree until the end of the document, but they have no children and no text, so the memory stays small for a large file.
