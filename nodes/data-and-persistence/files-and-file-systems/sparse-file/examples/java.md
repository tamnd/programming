```java run id=sparse-hint
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardOpenOption;

public class Main {
    public static void main(String[] args) throws Exception {
        Path dir = Files.createTempDirectory("sparse");
        Path file = dir.resolve("sparse.bin");

        // SPARSE is a hint for CREATE_NEW. A file system without holes ignores it.
        try (FileChannel ch = FileChannel.open(file, StandardOpenOption.CREATE_NEW,
                StandardOpenOption.WRITE, StandardOpenOption.SPARSE)) {
            ch.write(ByteBuffer.wrap("end".getBytes(StandardCharsets.US_ASCII)), 64 << 20);
        }

        byte[] data = Files.readAllBytes(file);
        System.out.println("size:       " + Files.size(file));
        System.out.println("first byte: " + data[0]);
        System.out.println("last bytes: " + new String(data, data.length - 3, 3, StandardCharsets.US_ASCII));

        Files.delete(file);
        Files.delete(dir);
    }
}
```

```output for=sparse-hint
size:       67108867
first byte: 0
last bytes: end
```

- Verdict: stdlib. A write at a position of a `FileChannel` past the end leaves a hole, and `StandardOpenOption.SPARSE` is only a hint for a new file. Java has no call that gives the allocated size of a file, and no `SEEK_DATA` or `SEEK_HOLE`.
