```typescript run id=extensions
type Kind<T> = new (doc: Document) => T;

class Document {
  text: string;
  #extensions = new Map<Function, object>();
  constructor(text: string) {
    this.text = text;
  }
  addExtension(extension: object): void {
    this.#extensions.set(extension.constructor, extension);
  }
  getExtension<T>(kind: Kind<T>): T | null {
    return (this.#extensions.get(kind) as T | undefined) ?? null;
  }
}

class WordCount {
  doc: Document;
  constructor(doc: Document) {
    this.doc = doc;
  }
  count(): number {
    return this.doc.text.split(/\s+/).length;
  }
}

class Upper {
  doc: Document;
  constructor(doc: Document) {
    this.doc = doc;
  }
  text(): string {
    return this.doc.text.toUpperCase();
  }
}

function showUpper(doc: Document): void {
  const upper = doc.getExtension(Upper); // type: Upper | null
  console.log(upper === null ? "upper: not supported" : `upper: ${upper.text()}`);
}

const doc = new Document("the quick brown fox");
doc.addExtension(new WordCount(doc));
console.log(`words: ${doc.getExtension(WordCount)?.count()}`);
showUpper(doc);
doc.addExtension(new Upper(doc));
showUpper(doc);
```

```output
words: 4
upper: not supported
upper: THE QUICK BROWN FOX
```

- Verdict: idiom. A generic `getExtension<T>` takes the class and returns `T | null`, so the compiler makes the client handle a missing extension.
