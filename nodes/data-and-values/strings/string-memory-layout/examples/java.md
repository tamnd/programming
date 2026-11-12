```java run id=compact-string-encodings
import java.nio.charset.StandardCharsets;

public class Main {
  public static void main(String[] args) {
    // The same text in three encodings. Latin-1 cannot hold every character.
    for (String text : new String[] {"hello", "café", "日本", "😀"}) {
      boolean latin1 = StandardCharsets.ISO_8859_1.newEncoder().canEncode(text);
      System.out.printf("%s: chars=%d latin-1=%s utf-8=%d utf-16=%d%n",
          text, text.length(),
          latin1 ? String.valueOf(text.getBytes(StandardCharsets.ISO_8859_1).length) : "-",
          text.getBytes(StandardCharsets.UTF_8).length,
          text.getBytes(StandardCharsets.UTF_16LE).length);
    }

    // A String stores its length, so a null char is an ordinary char.
    String s = "a\0b";
    System.out.println(s.length() + " " + (int) s.charAt(1));

    // A lone surrogate is a valid String but not valid text.
    // getBytes replaces it with '?' (63).
    String lone = "😀".substring(0, 1);
    byte[] bytes = lone.getBytes(StandardCharsets.UTF_8);
    System.out.println(lone.length() + " " + bytes.length + " " + bytes[0]);
  }
}
```

```output for=compact-string-encodings
hello: chars=5 latin-1=5 utf-8=5 utf-16=10
café: chars=4 latin-1=4 utf-8=5 utf-16=8
日本: chars=2 latin-1=- utf-8=6 utf-16=4
😀: chars=2 latin-1=- utf-8=4 utf-16=4
3 0
1 1 63
```

- Verdict: native. `length()` counts UTF-16 `char` values, and the byte counts show the size in Latin-1, UTF-8 and UTF-16. Latin-1 can encode `hello` and `café`, which are the strings that compact strings store with one byte per character. A `String` stores its length, so a null `char` is data. A lone surrogate encodes to one `?` byte (63).
