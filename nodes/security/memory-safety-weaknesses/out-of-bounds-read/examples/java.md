```java wrong id=copy-of-range
import java.nio.charset.StandardCharsets;
import java.util.Arrays;

public class Main {
    // Returns the first claimed bytes of the payload.
    static byte[] echo(byte[] payload, int claimed) {
        return Arrays.copyOfRange(payload, 0, claimed);
    }

    public static void main(String[] args) {
        byte[] payload = "hello".getBytes(StandardCharsets.US_ASCII);
        byte[] reply = echo(payload, 64);
        System.out.println("reply length: " + reply.length);
        System.out.println("byte 10: " + reply[10]);
    }
}
```

```output for=copy-of-range
reply length: 64
byte 10: 0
```

`copyOfRange` accepts an end past the length of the array and fills the rest with zeros. No memory leaks, but the reply claims 64 bytes and the program does not notice that the input was wrong.

```java run id=check
import java.nio.charset.StandardCharsets;
import java.util.Arrays;

public class Main {
    // Returns the first claimed bytes of the payload.
    static byte[] echo(byte[] payload, int claimed) {
        if (claimed < 0 || claimed > payload.length) {
            throw new IllegalArgumentException("claimed " + claimed + " bytes, received " + payload.length);
        }
        return Arrays.copyOfRange(payload, 0, claimed);
    }

    public static void main(String[] args) {
        byte[] payload = "hello".getBytes(StandardCharsets.US_ASCII);
        for (int claimed : new int[] {5, 64}) {
            try {
                byte[] reply = echo(payload, claimed);
                System.out.println("claimed " + claimed + ": " + new String(reply, StandardCharsets.US_ASCII));
            } catch (IllegalArgumentException e) {
                System.out.println("discarded: " + e.getMessage());
            }
        }
    }
}
```

```output for=check
claimed 5: hello
discarded: claimed 64 bytes, received 5
```

- Verdict: native. Every array access is checked, and an index out of range throws `ArrayIndexOutOfBoundsException`.
- `Arrays.copyOfRange` and `Arrays.copyOf` pad a copy that is longer than the source. Compare a length from input with `length` first.
- Native code that the program calls through JNI has no such check.
