```java run id=forms
public class Main {
  static int toSignMagnitude(int n) { return n < 0 ? 0x80 | -n : n; }
  static int toOnes(int n) { return n < 0 ? ~-n & 0xFF : n; }
  static int fromSignMagnitude(int b) { return (b & 0x80) != 0 ? -(b & 0x7F) : b; }
  static int fromOnes(int b) { return (b & 0x80) != 0 ? -(~b & 0xFF) : b; }

  static String bin8(int b) {
    return String.format("%8s", Integer.toBinaryString(b)).replace(' ', '0');
  }

  public static void main(String[] args) {
    System.out.println("-43 in sign-magnitude: " + bin8(toSignMagnitude(-43)));
    System.out.println("-43 in ones' complement: " + bin8(toOnes(-43)));
    System.out.println("10101011 in sign-magnitude: " + fromSignMagnitude(0b10101011));
    System.out.println("11010100 in ones' complement: " + fromOnes(0b11010100));
    System.out.println("10000000 in sign-magnitude: " + fromSignMagnitude(0b10000000));
    System.out.println("11111111 in ones' complement: " + fromOnes(0b11111111));

    double z = -0.0;
    System.out.println("-0.0 == 0.0: " + (z == 0.0 ? "yes" : "no"));
    System.out.println("Double.compare(-0.0, 0.0): " + Double.compare(z, 0.0));
    System.out.println("1 / -0.0: " + 1 / z);
  }
}
```

```output for=forms
-43 in sign-magnitude: 10101011
-43 in ones' complement: 11010100
10101011 in sign-magnitude: -43
11010100 in ones' complement: -43
10000000 in sign-magnitude: 0
11111111 in ones' complement: 0
-0.0 == 0.0: yes
Double.compare(-0.0, 0.0): -1
1 / -0.0: -Infinity
```

- Verdict: idiom. Java's integral types are two's complement, so a program makes the other two forms by hand with `|`, `~` and `&`.
- `==` says that `-0.0` and `0.0` are equal, but `Double.compare` puts `-0.0` first.
