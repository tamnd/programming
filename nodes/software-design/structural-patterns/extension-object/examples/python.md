```python run id=extensions
class Document:
    def __init__(self, text):
        self.text = text
        self._extensions = {}

    def add_extension(self, extension):
        self._extensions[type(extension)] = extension

    def get_extension(self, kind):
        return self._extensions.get(kind)  # None if there is no such extension


class WordCount:
    def __init__(self, doc):
        self.doc = doc

    def count(self):
        return len(self.doc.text.split())


class Upper:
    def __init__(self, doc):
        self.doc = doc

    def text(self):
        return self.doc.text.upper()


def show_upper(doc):
    upper = doc.get_extension(Upper)
    print("upper: not supported" if upper is None else f"upper: {upper.text()}")


doc = Document("the quick brown fox")
doc.add_extension(WordCount(doc))
print(f"words: {doc.get_extension(WordCount).count()}")
show_upper(doc)
doc.add_extension(Upper(doc))
show_upper(doc)
```

```output
words: 4
upper: not supported
upper: THE QUICK BROWN FOX
```

- Verdict: idiom. A dictionary keyed by class holds the extensions, and `get` returns `None` for a missing one.
