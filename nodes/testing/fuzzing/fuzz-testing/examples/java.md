```java run id=seeds
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HexFormat;
import java.util.List;

public class Main {
    record Rec(int tag, byte[] value) {}

    /** Reads records of the form [tag][length][value]. */
    static List<Rec> parse(byte[] data) {
        List<Rec> records = new ArrayList<>();
        int i = 0;
        while (i < data.length) {
            if (data.length - i < 2) {
                throw new IllegalArgumentException("truncated header at offset " + i);
            }
            int tag = Byte.toUnsignedInt(data[i]);
            int length = Byte.toUnsignedInt(data[i + 1]); // byte is signed in Java
            if (data.length - i - 2 < length) {
                throw new IllegalArgumentException("truncated value at offset " + i);
            }
            records.add(new Rec(tag, Arrays.copyOfRange(data, i + 2, i + 2 + length)));
            i += 2 + length;
        }
        return records;
    }

    /** The fuzz target: any exception other than IllegalArgumentException is a bug. */
    static String fuzzOne(byte[] data) {
        try {
            return "records=" + parse(data).size();
        } catch (IllegalArgumentException e) {
            return "rejected: " + e.getMessage();
        }
    }

    public static void main(String[] args) {
        byte[][] seeds = {
            {}, {1, 2, 'h', 'i'}, {1, 2, 'h', 'i', 2, 0}, {'z'}, {'~', (byte) 0xe1},
        };
        HexFormat hex = HexFormat.ofDelimiter(" ");
        for (byte[] seed : seeds) {
            String shown = seed.length == 0 ? "(empty)" : hex.formatHex(seed);
            System.out.println(shown + ": " + fuzzOne(seed));
        }
        System.out.println("seed corpus: " + seeds.length + " inputs passed");
    }
}
```

```output
(empty): records=0
01 02 68 69: records=1
01 02 68 69 02 00: records=2
7a: rejected: truncated header at offset 0
7e e1: rejected: truncated value at offset 0
seed corpus: 5 inputs passed
```

The original parser throws `ArrayIndexOutOfBoundsException` on the input of one byte.

```java fails expect=exception:ArrayIndexOutOfBoundsException id=no-length-check
public class Main {
    /** Counts the records of the form [tag][length][value]. */
    static int parse(byte[] data) {
        int count = 0;
        int i = 0;
        while (i < data.length) {
            int length = Byte.toUnsignedInt(data[i + 1]); // no check that the length byte exists
            count++;
            i += 2 + length;
        }
        return count;
    }

    public static void main(String[] args) {
        byte[][] seeds = {
            {}, {1, 2, 'h', 'i'}, {1, 2, 'h', 'i', 2, 0}, {'z'}, {'~', (byte) 0xe1},
        };
        for (byte[] seed : seeds) {
            System.out.println("records=" + parse(seed));
        }
    }
}
```

- Verdict: library. Jazzer is a coverage-guided fuzzer for the JVM, based on libFuzzer. Under JUnit, it runs saved inputs in a regression mode and generates inputs in a fuzzing mode.
- Java checks every array index, so the bug is an exception that a fuzzer sees.
- `byte` is signed in Java, so a length byte of `0xe1` reads as -31. `Byte.toUnsignedInt` gives 225. Without it, the length -31 passes the check `data.length - i - 2 < length`, and `copyOfRange` throws `IllegalArgumentException`. That is the type of the documented rejection, so the harness would hide the bug.
- `IllegalArgumentException` is the documented rejection. Any other exception escapes from `fuzzOne`.
