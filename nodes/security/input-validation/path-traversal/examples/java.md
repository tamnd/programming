```java wrong id=resolve
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

public class Main {
    public static void main(String[] args) throws IOException {
        Path base = Files.createTempDirectory("traversal");
        Path publicDir = Files.createDirectory(base.resolve("public"));
        Files.writeString(publicDir.resolve("report.txt"), "quarterly report");
        Files.writeString(base.resolve("secret.txt"), "the secret key");

        System.out.println(Files.readString(publicDir.resolve("report.txt")));
        System.out.println(Files.readString(publicDir.resolve("../secret.txt")));
        System.out.println(Files.readString(publicDir.resolve(base.resolve("secret.txt").toString())));
    }
}
```

```output for=resolve
quarterly report
the secret key
the secret key
```

```java run id=normalize
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;

public class Main {
    static Path root;

    static String readFile(String name) throws IOException {
        Path path = root.resolve(name).normalize();
        if (!path.startsWith(root) || !path.toRealPath().startsWith(root)) {
            return "refused, the path is outside the public directory";
        }
        return Files.readString(path);
    }

    public static void main(String[] args) throws IOException {
        Path base = Files.createTempDirectory("traversal").toRealPath();
        root = Files.createDirectory(base.resolve("public"));
        Files.writeString(root.resolve("report.txt"), "quarterly report");
        Files.writeString(base.resolve("secret.txt"), "the secret key");
        Files.createSymbolicLink(root.resolve("link.txt"), Path.of("../secret.txt"));

        System.out.println(readFile("report.txt"));
        System.out.println(readFile("../secret.txt"));
        System.out.println(readFile(base.resolve("secret.txt").toString()));
        System.out.println(readFile("link.txt"));
        System.out.println(Path.of("/srv/public-old/a.txt").startsWith(Path.of("/srv/public")));
    }
}
```

```output for=normalize
quarterly report
refused, the path is outside the public directory
refused, the path is outside the public directory
refused, the path is outside the public directory
false
```

- Verdict: stdlib. `Path.resolve` joins a name to a directory, and returns the other path as it is when the other path is absolute. `normalize` removes `.` and `..`. `Path.startsWith` compares whole name elements, so `/srv/public-old` does not start with `/srv/public`.
- `normalize` works on the text and does not know about symbolic links. `toRealPath` follows them, so the second example checks both. `toRealPath` fails with `NoSuchFileException` when the file does not exist.
- `String.startsWith` on path strings has the sibling problem. Use `Path.startsWith`.
- CodeQL queries `java/path-injection` and `java/zipslip` follow input and archive entry names to file operations.
