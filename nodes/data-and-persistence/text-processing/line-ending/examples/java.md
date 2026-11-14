```java run id=read-line
import java.io.BufferedReader;
import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class Main {
    // show writes CR as \r so that the output is readable.
    static String show(Object o) {
        return o.toString().replace("\r", "\\r");
    }

    public static void main(String[] args) throws IOException {
        String text = "one\ntwo\r\nthree\rfour";

        // readLine ends a line at LF, CR or CRLF, and removes the ending.
        List<String> lines = new ArrayList<>();
        try (BufferedReader reader = new BufferedReader(new StringReader(text))) {
            for (String line; (line = reader.readLine()) != null; ) {
                lines.add(line);
            }
        }
        System.out.println(lines);

        // split("\n") keeps the CR of a CRLF.
        System.out.println(show(Arrays.asList(text.split("\n"))));

        // System.lineSeparator() and %n give the ending of the platform.
        System.out.println("lineSeparator is LF: " + System.lineSeparator().equals("\n"));
        System.out.print(show(String.format("a%nb%n").replace("\n", "\\n")) + "\n");
    }
}
```

```output for=read-line
[one, two, three, four]
[one, two\r, three\rfour]
lineSeparator is LF: true
a\nb\n
```

- Verdict: stdlib. `BufferedReader.readLine` ends a line at LF, CR or CRLF and removes the ending. `System.lineSeparator()` and the `%n` conversion of `String.format` give the ending of the platform, which is LF on Linux.
