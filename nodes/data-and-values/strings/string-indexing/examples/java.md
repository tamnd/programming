```java run id=charat-and-offset-by-code-points
public class Main {
  public static void main(String[] args) {
    String s = "naïve 😀";

    // charAt gives a char, which is one UTF-16 code unit.
    System.out.println(s.charAt(2) + " " + s.length() + " " + Character.isHighSurrogate(s.charAt(6)));

    // codePointAt reads the whole code point that starts at a char index.
    System.out.println(Integer.toHexString(s.codePointAt(6)) + " " + Character.charCount(s.codePointAt(6)));

    // offsetByCodePoints turns a code point index into a char index.
    int seventh = s.offsetByCodePoints(0, 6);
    System.out.println(seventh + " " + s.substring(seventh));

    // An index out of range throws.
    try {
      s.charAt(20);
    } catch (StringIndexOutOfBoundsException e) {
      System.out.println(e.getClass().getSimpleName());
    }

    // A substring can cut a surrogate pair in half.
    String half = s.substring(0, 7);
    System.out.println(half.length() + " " + Character.isSurrogate(half.charAt(6)));
  }
}
```

```output for=charat-and-offset-by-code-points
ï 8 true
1f600 2
6 😀
StringIndexOutOfBoundsException
7 true
```

- Verdict: native. `charAt(2)` is `ï`, and `charAt(6)` is a high surrogate. `codePointAt(6)` reads the emoji, which takes 2 `char` values. `offsetByCodePoints(0, 6)` turns the code point index 6 into the `char` index 6. `charAt(20)` throws `StringIndexOutOfBoundsException`, and `substring(0, 7)` ends with half of the surrogate pair.
