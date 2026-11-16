```java run id=unsigned-methods
public class Main {
  static String yesNo(boolean b) {
    return b ? "yes" : "no";
  }

  public static void main(String[] args) {
    byte b = (byte) 200;
    System.out.println("(byte) 200: " + b);
    System.out.println("Byte.toUnsignedInt(b): " + Byte.toUnsignedInt(b));

    int x = -1;
    System.out.println("Integer.toUnsignedString(-1): " + Integer.toUnsignedString(x));
    System.out.println("-1 < 1 as signed: " + yesNo(x < 1));
    System.out.println("-1 < 1 as unsigned: " + yesNo(Integer.compareUnsigned(x, 1) < 0));
    System.out.println("Integer.divideUnsigned(-2, 2): " + Integer.divideUnsigned(-2, 2));
  }
}
```

```output for=unsigned-methods
(byte) 200: -56
Byte.toUnsignedInt(b): 200
Integer.toUnsignedString(-1): 4294967295
-1 < 1 as signed: yes
-1 < 1 as unsigned: no
Integer.divideUnsigned(-2, 2): 2147483647
```

- Verdict: stdlib. Java has no unsigned `int` or `long`, and `byte` is signed. Only `char` is unsigned.
- The classes `Byte`, `Integer` and `Long` read a signed value as unsigned. `Integer.compareUnsigned` and `Integer.divideUnsigned` give the unsigned result for the same bits.
- Addition, subtraction and multiplication need no unsigned method, because they give the same bits for both readings.
