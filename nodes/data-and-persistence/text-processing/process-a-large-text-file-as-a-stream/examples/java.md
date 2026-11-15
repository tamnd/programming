```java run id=java-stream
import java.io.BufferedWriter;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.IntSummaryStatistics;
import java.util.stream.Stream;

public class Main {
    static IntSummaryStatistics stream(Path path) throws IOException {
        // Files.lines reads the lines as the stream needs them. The try closes the file.
        try (Stream<String> lines = Files.lines(path, StandardCharsets.UTF_8)) {
            return lines.mapToInt(Integer::parseInt).summaryStatistics();
        }
    }

    public static void main(String[] args) throws IOException {
        Path dir = Files.createTempDirectory("stream");
        Path path = dir.resolve("numbers.txt");
        try (BufferedWriter w = Files.newBufferedWriter(path, StandardCharsets.UTF_8)) {
            for (int i = 0; i < 1_000_000; i++) {
                w.write(Integer.toString(i % 1000));
                w.write('\n');
            }
        }
        IntSummaryStatistics s = stream(path);
        System.out.println("lines: " + s.getCount() + " sum: " + s.getSum() + " max: " + s.getMax());
        Files.delete(path);
        Files.delete(dir);
    }
}
```

```output
lines: 1000000 sum: 499500000 max: 999
```

`Files.lines` returns a `Stream<String>` that reads the lines as the stream needs them, and `summaryStatistics` keeps the count, the sum and the maximum. The `try` with resources closes the stream, and so the file.
