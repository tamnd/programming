```java run id=integer-binary-methods
public class Main {
    public static void main(String[] args) {
        int n = 0b1010_1100; // binary literal; the underscore groups digits
        System.out.println(n + " " + Integer.toBinaryString(n) + " " + Integer.toOctalString(n) + " " + Integer.toHexString(n));
        System.out.println(Integer.parseInt("101101", 2) + " " + Integer.parseInt("-ff", 16) + " " + Long.parseLong("777", 8));

        // toBinaryString shows all 32 bits of a negative int, in two's complement.
        System.out.println(Integer.toBinaryString(-5));
        System.out.println(Integer.parseUnsignedInt("11111111111111111111111111111011", 2));
        System.out.println(Integer.bitCount(n) + " " + Integer.numberOfLeadingZeros(n) + " " + Integer.highestOneBit(n));
        System.out.println(String.format("%8s", Integer.toBinaryString(5)).replace(' ', '0'));
    }
}
```

```output for=integer-binary-methods
172 10101100 254 ac
45 -255 511
11111111111111111111111111111011
-5
4 24 128
00000101
```

- Verdict: native. `Integer.toBinaryString` treats the int as unsigned, so −5 prints as 32 bits of two's complement, and `parseUnsignedInt` reads those bits back as −5. `parseInt` takes a radix and a sign. `bitCount`, `numberOfLeadingZeros` and `highestOneBit` work on the bits directly. Java has no format for binary, so padding needs `String.format` and `replace`.
