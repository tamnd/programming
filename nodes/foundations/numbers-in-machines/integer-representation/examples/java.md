```java run id=two-complement-int
public class Main {
    public static void main(String[] args) {
        byte b = (byte) 200; // the cast keeps the low 8 bits
        System.out.println(b + " " + Byte.toUnsignedInt(b));

        int max = Integer.MAX_VALUE;
        System.out.println((max + 1) + " " + Math.abs(Integer.MIN_VALUE)); // int arithmetic wraps
        try {
            Math.addExact(max, 1);
        } catch (ArithmeticException e) {
            System.out.println("addExact: " + e.getMessage());
        }

        // >> copies the sign bit; >>> shifts in zeros.
        System.out.println((-16 >> 2) + " " + (-16 >>> 28));
        // Java has no unsigned int type, but Integer has unsigned methods.
        System.out.println(Integer.toUnsignedString(-1) + " " + Integer.divideUnsigned(-1, 2) + " " + Integer.compareUnsigned(-1, 1));
        System.out.println((long) b + " " + (b & 0xFFL)); // widening keeps the sign; a mask does not
    }
}
```

```output for=two-complement-int
-56 200
-2147483648 -2147483648
addExact: integer overflow
-4 15
4294967295 2147483647 1
-56 200
```

- Verdict: native. Java's integer types are signed two's complement, and `int` arithmetic wraps with no error, so `Integer.MAX_VALUE + 1` is the most negative `int` and `Math.abs(Integer.MIN_VALUE)` is negative. `Math.addExact` throws `ArithmeticException` instead. The cast `(byte) 200` keeps the low 8 bits, which read as −56, and `Byte.toUnsignedInt` or `& 0xFF` gives 200 back. Java has no unsigned `int`, but `Integer` has unsigned methods, and `>>>` is the logical right shift.
