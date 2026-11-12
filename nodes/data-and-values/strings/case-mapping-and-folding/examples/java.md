```java run id=locale-root-and-turkish
import java.util.Locale;

public class Main {
  public static void main(String[] args) {
    Locale tr = Locale.forLanguageTag("tr");

    // toUpperCase maps "ß" to "SS", so the length changes.
    String s = "straße";
    String up = s.toUpperCase(Locale.ROOT);
    System.out.println(up + " " + s.length() + " " + up.length());

    // The Turkish locale changes the mappings of i and I.
    System.out.println("title".toUpperCase(Locale.ROOT) + " " + "title".toUpperCase(tr));
    System.out.println("TITLE".toLowerCase(Locale.ROOT) + " " + "TITLE".toLowerCase(tr));

    // equalsIgnoreCase compares code point by code point, so the lengths must match.
    System.out.println("straße".equalsIgnoreCase("STRASSE") + " " + "ΌΣΟΣ".equalsIgnoreCase("όσος"));

    // Character has a title case mapping that differs from upper case for "ǳ".
    char dz = 'ǳ';
    System.out.println(Character.toUpperCase(dz) + " " + Character.toTitleCase(dz));

    // A char method cannot expand: Character.toUpperCase('ß') returns 'ß'.
    System.out.println(Character.toUpperCase('ß'));
  }
}
```

```output for=locale-root-and-turkish
STRASSE 6 7
TITLE TİTLE
title tıtle
false true
Ǳ ǲ
ß
```

- Verdict: native. `toUpperCase(Locale.ROOT)` maps `ß` to `SS`, so the length goes from 6 to 7. The Turkish locale gives `TİTLE` and `tıtle`. `equalsIgnoreCase` compares code point by code point, so `straße` and `STRASSE` are not equal because their lengths differ, but the two sigma forms are equal. `Character.toTitleCase` gives `ǲ`, and `Character.toUpperCase('ß')` returns `ß`, because one `char` cannot become two.
