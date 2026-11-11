```java run id=shifts-and-signed-bytes
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.HexFormat;

public class Main {
    static final int READ = 1, WRITE = 1 << 1, EXEC = 1 << 2;

    public static void main(String[] args) {
        int perm = READ | WRITE;
        perm &= ~WRITE;
        perm ^= EXEC;
        System.out.println(Integer.toBinaryString(perm) + " " + Integer.bitCount(perm));

        System.out.println((-8 >> 1) + " " + (-8 >>> 28) + " " + (1 << 32) + " " + (1L << 32));

        byte b = (byte) 0xF0;  // byte is signed
        System.out.println(b + " " + (b & 0xFF) + " " + Integer.toHexString(b));

        byte[] le = ByteBuffer.allocate(4).order(ByteOrder.LITTLE_ENDIAN).putInt(0x12345678).array();
        System.out.println(HexFormat.of().formatHex(le) + " " + Integer.toHexString(Integer.reverseBytes(0x12345678)));
    }
}
```

```output for=shifts-and-signed-bytes
101 2
-4 15 1 4294967296
-16 240 fffffff0
78563412 78563412
```

- Verdict: native. Java has `&`, `|`, `^`, `~`, `<<`, the arithmetic right shift `>>` and the logical right shift `>>>` on `int` and `long`. A shift of an `int` uses only the low 5 bits of the count, so `1 << 32` is `1`, and a shift of a `long` uses the low 6 bits. `Integer` and `Long` have `bitCount`, `toBinaryString`, `reverseBytes` and other bit methods.
- Java's `byte` is signed, so `(byte) 0xF0` is `-16` and widens to `0xFFFFFFF0`. Mask with `& 0xFF` to read a byte as a number from 0 to 255. `ByteBuffer` reads and writes in big-endian order by default, and `order` changes it.
