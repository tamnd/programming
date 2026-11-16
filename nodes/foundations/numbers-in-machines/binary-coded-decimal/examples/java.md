```java run id=packed
import java.util.HexFormat;

public class Main {
  static byte[] toPacked(long n) {
    String s = Long.toString(Math.abs(n));
    int nibbles = s.length() + 1;
    int size = (nibbles + 1) / 2;
    byte[] out = new byte[size];
    int pos = size * 2 - 1;
    out[pos / 2] |= (byte) (n < 0 ? 0xD : 0xC);
    for (int i = s.length() - 1; i >= 0; i--) {
      pos--;
      int d = s.charAt(i) - '0';
      out[pos / 2] |= (byte) (pos % 2 == 0 ? d << 4 : d);
    }
    return out;
  }

  static long fromPacked(byte[] b) {
    long value = 0;
    for (int i = 0; i < b.length; i++) {
      int hi = (b[i] >> 4) & 0x0F;
      int lo = b[i] & 0x0F;
      if (hi > 9 || (i < b.length - 1 && lo > 9)) {
        throw new IllegalArgumentException("not packed BCD");
      }
      value = value * 10 + hi;
      if (i < b.length - 1) {
        value = value * 10 + lo;
      } else if (lo == 0xD) {
        value = -value;
      }
    }
    return value;
  }

  public static void main(String[] args) {
    HexFormat hex = HexFormat.ofDelimiter(" ").withUpperCase();
    byte[] p = toPacked(-12345);
    System.out.println("packed -12345: " + hex.formatHex(p));
    System.out.println("back: " + fromPacked(p));
    System.out.println("packed 2024: " + hex.formatHex(toPacked(2024)));
    try {
      fromPacked(new byte[] {0x1A, 0x3C});
    } catch (IllegalArgumentException e) {
      System.out.println("1A 3C: " + e.getMessage());
    }
  }
}
```

```output for=packed
packed -12345: 12 34 5D
back: -12345
packed 2024: 02 02 4C
1A 3C: not packed BCD
```

- Verdict: idiom. Java has no BCD type. The block packs the digits from the right, so the sign nibble is last and a leading 0 nibble fills an even count of digits.
- Java's `byte` is signed, so the decoder masks with `0x0F` after the shift.
