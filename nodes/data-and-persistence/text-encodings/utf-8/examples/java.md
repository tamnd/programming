```java run id=lengths
import java.nio.charset.StandardCharsets;
import java.text.BreakIterator;
import java.text.Normalizer;

public class Main {
    public static void main(String[] args) {
        String text = "nai\u0308ve \uD83D\uDC69\u200D\uD83D\uDCBB";
        System.out.println("UTF-8 bytes " + text.getBytes(StandardCharsets.UTF_8).length);
        System.out.println("UTF-16 chars " + text.length());
        System.out.println("code points " + text.codePointCount(0, text.length()));

        BreakIterator graphemes = BreakIterator.getCharacterInstance();
        graphemes.setText(text);
        int count = 0;
        while (graphemes.next() != BreakIterator.DONE) {
            count++;
        }
        System.out.println("grapheme clusters " + count);

        String composed = Normalizer.normalize(text, Normalizer.Form.NFC);
        System.out.println("after NFC " + composed.length() + " " + composed.getBytes(StandardCharsets.UTF_8).length);
    }
}
```

```output for=lengths
UTF-8 bytes 19
UTF-16 chars 12
code points 10
grapheme clusters 7
after NFC 11 18
```

```java run id=decode
import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.charset.CharacterCodingException;
import java.nio.charset.CodingErrorAction;
import java.nio.charset.StandardCharsets;
import java.util.HexFormat;

public class Main {
    public static void main(String[] args) throws IOException {
        byte[] bad = {0x61, (byte) 0xC0, (byte) 0xAF, 0x62};
        String replaced = new String(bad, StandardCharsets.UTF_8);
        System.out.println(replaced.length() + " " + replaced.chars().filter(c -> c == 0xFFFD).count());

        try {
            StandardCharsets.UTF_8.newDecoder()
                .onMalformedInput(CodingErrorAction.REPORT)
                .decode(ByteBuffer.wrap(bad));
        } catch (CharacterCodingException error) {
            System.out.println(error);
        }

        HexFormat hex = HexFormat.ofDelimiter(" ");
        String sample = "\u0000\uD834\uDD1E";
        System.out.println("UTF-8          " + hex.formatHex(sample.getBytes(StandardCharsets.UTF_8)));

        ByteArrayOutputStream buffer = new ByteArrayOutputStream();
        new DataOutputStream(buffer).writeUTF(sample);
        System.out.println("modified UTF-8 " + hex.formatHex(buffer.toByteArray()));
    }
}
```

```output for=decode
4 2
java.nio.charset.MalformedInputException: Input length = 1
UTF-8          00 f0 9d 84 9e
modified UTF-8 00 08 c0 80 ed a0 b4 ed b4 9e
```

- Verdict: stdlib. `String.getBytes(StandardCharsets.UTF_8)` and the constructor `new String(bytes, StandardCharsets.UTF_8)` convert between strings and UTF-8.
- A Java `String` is a sequence of UTF-16 `char` values. `length` counts code units, and `codePointCount` counts code points. `BreakIterator.getCharacterInstance` finds the boundaries of user characters, and `Normalizer` does the four normalization forms.
- The `String` constructor replaces bad input with U+FFFD and reports nothing. A `CharsetDecoder` with `CodingErrorAction.REPORT` throws `MalformedInputException`.
- `DataOutputStream.writeUTF` writes modified UTF-8, not UTF-8. It starts with a two-byte length. It writes U+0000 as `C0 80`, and it writes a character outside the BMP as two three-byte surrogates. Class files and JNI also use this form. Do not send it to a program that expects UTF-8.
- JEP 400 made UTF-8 the default charset of the standard Java APIs, except for console I/O. Older releases took the default from the platform, so name the charset in each call.
