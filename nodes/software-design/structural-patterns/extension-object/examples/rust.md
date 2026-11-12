```rust run id=extensions
use std::any::{Any, TypeId};
use std::collections::HashMap;

struct Document {
    text: String,
    extensions: HashMap<TypeId, Box<dyn Any>>,
}

impl Document {
    fn new(text: &str) -> Document {
        Document { text: text.to_string(), extensions: HashMap::new() }
    }
    fn add_extension<T: Any>(&mut self, extension: T) {
        self.extensions.insert(TypeId::of::<T>(), Box::new(extension));
    }
    fn get_extension<T: Any>(&self) -> Option<&T> {
        self.extensions.get(&TypeId::of::<T>())?.downcast_ref::<T>()
    }
}

// The extensions take the document as an argument, so that they do not
// hold a reference to the object that owns them.
struct WordCount;

impl WordCount {
    fn count(&self, doc: &Document) -> usize {
        doc.text.split_whitespace().count()
    }
}

struct Upper;

impl Upper {
    fn text(&self, doc: &Document) -> String {
        doc.text.to_uppercase()
    }
}

fn show_upper(doc: &Document) {
    match doc.get_extension::<Upper>() {
        Some(upper) => println!("upper: {}", upper.text(doc)),
        None => println!("upper: not supported"),
    }
}

fn main() {
    let mut doc = Document::new("the quick brown fox");
    doc.add_extension(WordCount);
    let words = doc.get_extension::<WordCount>().map(|w| w.count(&doc));
    println!("words: {}", words.unwrap_or(0));
    show_upper(&doc);
    doc.add_extension(Upper);
    show_upper(&doc);
}
```

```output
words: 4
upper: not supported
upper: THE QUICK BROWN FOX
```

- Verdict: idiom. `TypeId` is the key, and `downcast_ref` on `dyn Any` gives back the typed extension as an `Option`. An extension that held a reference to its owner would need shared ownership, so here the owner is passed to each method.
