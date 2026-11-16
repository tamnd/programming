```java run id=tiny
public class Main {
  static String yes(boolean b) {
    return b ? "yes" : "no";
  }

  public static void main(String[] args) {
    System.out.println("MIN_NORMAL: " + Double.MIN_NORMAL);
    System.out.println("MIN_VALUE:  " + Double.MIN_VALUE);
    System.out.println("Float.MIN_VALUE: " + Float.MIN_VALUE);

    double half = Double.MIN_NORMAL / 2;
    System.out.println("getExponent(MIN_NORMAL):     " + Math.getExponent(Double.MIN_NORMAL));
    System.out.println("getExponent(MIN_NORMAL / 2): " + Math.getExponent(half));
    System.out.println("getExponent(0.0):            " + Math.getExponent(0.0));

    System.out.println("bits of MIN_VALUE:  " + Long.toHexString(Double.doubleToRawLongBits(Double.MIN_VALUE)));
    System.out.println("bits of MIN_NORMAL: " + Long.toHexString(Double.doubleToRawLongBits(Double.MIN_NORMAL)));

    double x = 1.25 * Double.MIN_NORMAL;
    System.out.println("x - MIN_NORMAL: " + (x - Double.MIN_NORMAL));
    System.out.println("MIN_VALUE / 2 == 0: " + yes(Double.MIN_VALUE / 2 == 0));
  }
}
```

```output for=tiny
MIN_NORMAL: 2.2250738585072014E-308
MIN_VALUE:  4.9E-324
Float.MIN_VALUE: 1.4E-45
getExponent(MIN_NORMAL):     -1022
getExponent(MIN_NORMAL / 2): -1023
getExponent(0.0):            -1023
bits of MIN_VALUE:  1
bits of MIN_NORMAL: 10000000000000
x - MIN_NORMAL: 5.562684646268003E-309
MIN_VALUE / 2 == 0: yes
```

- Verdict: native. Java `double` follows IEEE 754 and has subnormals.
- `Double.MIN_VALUE` is the smallest subnormal. The name suggests the smallest normal value, which is `Double.MIN_NORMAL`.
- `Math.getExponent` gives `MIN_EXPONENT - 1`, that is -1023, for zero and for every subnormal.
- The bits of `MIN_VALUE` are 1: an exponent field of 0 and only the last fraction bit set.
