```javascript run id=extensions
class Document {
  #extensions = new Map(); // keyed by class
  constructor(text) {
    this.text = text;
  }
  addExtension(extension) {
    this.#extensions.set(extension.constructor, extension);
  }
  getExtension(kind) {
    return this.#extensions.get(kind) ?? null;
  }
}

class WordCount {
  constructor(doc) {
    this.doc = doc;
  }
  count() {
    return this.doc.text.split(/\s+/).length;
  }
}

class Upper {
  constructor(doc) {
    this.doc = doc;
  }
  text() {
    return this.doc.text.toUpperCase();
  }
}

function showUpper(doc) {
  const upper = doc.getExtension(Upper);
  console.log(upper === null ? "upper: not supported" : `upper: ${upper.text()}`);
}

const doc = new Document("the quick brown fox");
doc.addExtension(new WordCount(doc));
console.log(`words: ${doc.getExtension(WordCount).count()}`);
showUpper(doc);
doc.addExtension(new Upper(doc));
showUpper(doc);
```

```output
words: 4
upper: not supported
upper: THE QUICK BROWN FOX
```

- Verdict: idiom. A `Map` can use a class as its key, so the lookup is by type.
