```java run id=break-iterator-and-regex-x
import java.text.BreakIterator;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Main {
  public static void main(String[] args) {
    String t = "cafe\u0301 " + Character.toString(0x1F1EB) + Character.toString(0x1F1F7)
        + " " + Character.toString(0x1F469) + Character.toString(0x1F3FD) + "\u200D"
        + Character.toString(0x1F692);

    // length() counts UTF-16 code units, and codePoints() counts code points.
    System.out.println(t.length() + " " + t.codePoints().count());

    // A character BreakIterator finds extended grapheme cluster boundaries.
    BreakIterator it = BreakIterator.getCharacterInstance();
    it.setText(t);
    List<Integer> starts = new ArrayList<>();
    for (int b = it.first(); b != BreakIterator.DONE; b = it.next()) {
      starts.add(b);
    }
    System.out.println((starts.size() - 1) + " " + starts);

    // The regex \X matches one extended grapheme cluster.
    Matcher m = Pattern.compile("\\X").matcher(t);
    int n = 0;
    String last = "";
    while (m.find()) {
      n++;
      last = m.group();
    }
    System.out.println(n + " " + last.length() + " " + last.codePointCount(0, last.length()));
  }
}
```

```output for=break-iterator-and-regex-x
18 13
8 [0, 1, 2, 3, 5, 6, 10, 11, 18]
8 7 4
```

- Verdict: stdlib. `length()` is 18 and `codePoints().count()` is 13. The character `BreakIterator` gives 9 boundaries, so 8 clusters, and they are `char` indexes. The regex `\X` finds the same 8 clusters, and the last one is 7 `char` values and 4 code points.
