```java run id=chars-and-code-points
import java.nio.charset.StandardCharsets;
import java.text.Normalizer;
import java.util.regex.Pattern;

public class Main {
    public static void main(String[] args) {
        String s = "cafe\u0301 \uD83D\uDC4D\uD83C\uDFFD"; // the emoji as surrogate pairs
        long graphemes = Pattern.compile("\\X").matcher(s).results().count();
        System.out.println(s.length() + " " + s.codePointCount(0, s.length()) + " "
                + graphemes + " " + s.getBytes(StandardCharsets.UTF_8).length);

        System.out.println(Integer.toHexString(s.charAt(6)) + " " + Integer.toHexString(s.codePointAt(6))
                + " " + Character.isSurrogate(s.charAt(6)));

        String nfc = Normalizer.normalize(s, Normalizer.Form.NFC);
        System.out.println(nfc.length() + " " + nfc.equals("caf\u00e9 \uD83D\uDC4D\uD83C\uDFFD"));
        System.out.println("Stra\u00dfe".toUpperCase(java.util.Locale.ROOT));
    }
}
```

```output for=chars-and-code-points
10 8 6 15
d83d 1f44d true
9 true
STRASSE
```

- Verdict: native. A Java `String` holds UTF-16 code units, and `char` is one unit, not one character. `length` counts units, `codePointCount` counts code points, and the regular expression `\X` matches one extended grapheme cluster, so counting its matches gives 6. Methods such as `codePointAt` and `Character.isSurrogate` deal with the pairs that encode characters outside the Basic Multilingual Plane.
- `java.text.Normalizer` converts between normalization forms. Case conversion depends on the locale, so pass `Locale.ROOT` for text that is not shown to a user.
