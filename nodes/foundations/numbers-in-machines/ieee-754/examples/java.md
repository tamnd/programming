```java run id=fields
public class Main {
    public static void main(String[] args) {
        for (double x : new double[] {1.0, -2.5, 0.1}) {
            short b16 = Float.floatToFloat16((float) x);
            int b32 = Float.floatToRawIntBits((float) x);
            long b64 = Double.doubleToRawLongBits(x);
            System.out.printf("%4s binary16 %04x binary32 %08x binary64 %016x%n", x, b16, b32, b64);
        }

        long b = Double.doubleToRawLongBits(-2.5);
        System.out.printf("-2.5: sign %d, exponent %d, fraction %x%n",
                b >>> 63, ((b >>> 52) & 0x7ff) - 1023, b & 0xfffffffffffffL);

        for (float f : new float[] {0.1f, 65504f, 65520f}) {
            System.out.println("binary16 of " + f + ": " + Float.float16ToFloat(Float.floatToFloat16(f)));
        }
    }
}
```

```output for=fields
 1.0 binary16 3c00 binary32 3f800000 binary64 3ff0000000000000
-2.5 binary16 c100 binary32 c0200000 binary64 c004000000000000
 0.1 binary16 2e66 binary32 3dcccccd binary64 3fb999999999999a
-2.5: sign 1, exponent 1, fraction 4000000000000
binary16 of 0.1: 0.099975586
binary16 of 65504.0: 65504.0
binary16 of 65520.0: Infinity
```

- Verdict: native. Java `float` is binary32 and `double` is binary64.
- `Float.floatToRawIntBits` and `Double.doubleToRawLongBits` give the bits. The "raw" forms keep the bits of a NaN as they are.
- Java has no binary16 type. `Float.floatToFloat16` rounds a `float` to binary16 with ties to even and stores the 16 bits in a `short`. `Float.float16ToFloat` converts the bits back.
- The format `%x` prints a negative `short` as an unsigned 16-bit value, so the bits of -2.5 print as `c100`.
