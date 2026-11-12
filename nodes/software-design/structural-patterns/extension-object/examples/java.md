```java run id=extensions
import java.util.HashMap;
import java.util.Map;

class Document {
    final String text;
    private final Map<Class<?>, Object> extensions = new HashMap<>();

    Document(String text) { this.text = text; }

    void addExtension(Object extension) { extensions.put(extension.getClass(), extension); }

    <T> T getExtension(Class<T> kind) {
        return kind.cast(extensions.get(kind)); // null if there is no such extension
    }
}

class WordCount {
    private final Document doc;
    WordCount(Document doc) { this.doc = doc; }
    int count() { return doc.text.split("\\s+").length; }
}

class Upper {
    private final Document doc;
    Upper(Document doc) { this.doc = doc; }
    String text() { return doc.text.toUpperCase(); }
}

public class Main {
    static void showUpper(Document doc) {
        Upper upper = doc.getExtension(Upper.class);
        System.out.println(upper == null ? "upper: not supported" : "upper: " + upper.text());
    }

    public static void main(String[] args) {
        Document doc = new Document("the quick brown fox");
        doc.addExtension(new WordCount(doc));
        System.out.println("words: " + doc.getExtension(WordCount.class).count());
        showUpper(doc);
        doc.addExtension(new Upper(doc));
        showUpper(doc);
    }
}
```

```output
words: 4
upper: not supported
upper: THE QUICK BROWN FOX
```

- Verdict: idiom. The lookup takes a `Class<T>` and returns a `T` or null, which is the signature of `getAdapter` in Eclipse's `IAdaptable`.
