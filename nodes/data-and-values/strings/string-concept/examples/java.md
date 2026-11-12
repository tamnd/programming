```java run id=string-and-stringbuilder
import java.nio.charset.StandardCharsets;

public class Main {
  public static void main(String[] args) {
    // A String holds UTF-16 code units. length() counts them.
    String s = "naïve 😀";
    System.out.println(s.length() + " " + s.codePointCount(0, s.length()));

    // A String is immutable. concat and toUpperCase return new strings.
    String t = s.concat("!");
    System.out.println(s + " | " + t + " | " + s.toUpperCase(java.util.Locale.ROOT));

    // StringBuilder is the mutable string type.
    StringBuilder sb = new StringBuilder("cat");
    sb.setCharAt(0, 'b');
    sb.append("s");
    System.out.println(sb + " " + sb.length());

    // getBytes encodes the text. new String decodes it.
    byte[] utf8 = s.getBytes(StandardCharsets.UTF_8);
    System.out.println(utf8.length + " " + new String(utf8, StandardCharsets.UTF_8).equals(s));

    // A char is one code unit, so the emoji is two chars.
    char hi = s.charAt(6);
    System.out.println(Character.isHighSurrogate(hi) + " " + Character.toChars(s.codePointAt(6)).length);
  }
}
```

```output for=string-and-stringbuilder
8 7
naïve 😀 | naïve 😀! | NAÏVE 😀
bats 4
11 true
true 2
```

- Verdict: native. `length()` counts UTF-16 code units and `codePointCount` counts code points, so the emoji counts twice in the first number. `concat` and `toUpperCase` return new strings, and `s` does not change. A `StringBuilder` changes in place with `setCharAt` and `append`. `getBytes` encodes to 11 UTF-8 bytes. `charAt(6)` is the high surrogate of the emoji.
