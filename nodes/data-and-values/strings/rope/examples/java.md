```java run id=char-sequence-and-regex
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Main {
  // A rope that implements CharSequence, so the regex engine and String methods accept it.
  static final class Rope implements CharSequence {
    final String s;
    final Rope left, right;
    final int n;

    Rope(String s) {
      this.s = s;
      this.left = null;
      this.right = null;
      this.n = s.length();
    }

    Rope(Rope left, Rope right) {
      this.s = null;
      this.left = left;
      this.right = right;
      this.n = left.n + right.n;
    }

    @Override
    public int length() {
      return n;
    }

    // charAt walks from the root to a leaf, so each call costs O(depth).
    @Override
    public char charAt(int i) {
      Rope t = this;
      while (t.s == null) {
        if (i < t.left.n) {
          t = t.left;
        } else {
          i -= t.left.n;
          t = t.right;
        }
      }
      return t.s.charAt(i);
    }

    @Override
    public CharSequence subSequence(int start, int end) {
      return toString().substring(start, end);
    }

    void appendTo(StringBuilder sb) {
      if (s != null) {
        sb.append(s);
      } else {
        left.appendTo(sb);
        right.appendTo(sb);
      }
    }

    @Override
    public String toString() {
      StringBuilder sb = new StringBuilder(n);
      appendTo(sb);
      return sb.toString();
    }
  }

  public static void main(String[] args) {
    Rope r = new Rope(new Rope(new Rope("The quick "), new Rope("brown fox ")), new Rope("jumps over the lazy dog"));
    System.out.println(r.length() + " " + r.charAt(10));

    // Pattern.matcher takes any CharSequence, so it reads the rope without a copy.
    Matcher m = Pattern.compile("\\b\\w{5}\\b").matcher(r);
    List<String> words = new ArrayList<>();
    while (m.find()) {
      words.add(m.group());
    }
    System.out.println(String.join(",", words));

    System.out.println("The quick brown fox jumps over the lazy dog".contentEquals(r));
  }
}
```

```output for=char-sequence-and-regex
43 b
quick,brown,jumps
true
```

- Verdict: idiom. Java SE has no rope. The class implements `CharSequence`, so `Pattern.matcher` and `String.contentEquals` accept it without a conversion to `String`. Each `charAt` call walks from the root to a leaf, so a regex scan costs O(n log n) on a rope and O(n) on a `String`.
