```java run id=links-with-nio
import java.nio.file.Files;
import java.nio.file.LinkOption;
import java.nio.file.Path;

public class Main {
    public static void main(String[] args) throws Exception {
        Path data = Path.of("data.txt");
        Files.writeString(data, "alpha\n");
        Path hard = Files.createLink(Path.of("hard.txt"), data);
        Path soft = Files.createSymbolicLink(Path.of("soft.txt"), Path.of("data.txt"));

        System.out.println("same file:  " + Files.isSameFile(hard, soft));
        System.out.println("symbolic:   " + Files.isSymbolicLink(soft) + " " + Files.isSymbolicLink(hard));
        System.out.println("target:     " + Files.readSymbolicLink(soft));

        Files.delete(data);
        System.out.println("hard reads: " + Files.readString(hard).strip());
        System.out.println("exists:     " + Files.exists(soft));
        System.out.println("link there: " + Files.exists(soft, LinkOption.NOFOLLOW_LINKS));
    }
}
```

```output for=links-with-nio
same file:  true
symbolic:   true false
target:     data.txt
hard reads: alpha
exists:     false
link there: true
```

- Verdict: stdlib. `Files.createLink` and `Files.createSymbolicLink` are optional operations, so a file system that has no links throws `UnsupportedOperationException`. `Files.exists` follows a link, and `LinkOption.NOFOLLOW_LINKS` asks about the link itself.
