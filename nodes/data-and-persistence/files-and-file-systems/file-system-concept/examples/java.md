```java run id=zip-file-system
import java.io.IOException;
import java.net.URI;
import java.nio.file.FileStore;
import java.nio.file.FileSystem;
import java.nio.file.FileSystems;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Map;

public class Main {
  public static void main(String[] args) throws IOException {
    Path dir = Files.createDirectories(Path.of("tree", "src"));
    Path main = Files.writeString(dir.resolve("main.txt"), "main\n");

    // A hard link is a second name. isSameFile compares the files, not the names.
    Path link = Files.createLink(Path.of("tree", "main-link.txt"), main);
    System.out.println("same file: " + Files.isSameFile(main, link));
    // The "unix" attribute view gives the inode number and the link count.
    System.out.println("links: " + Files.getAttribute(main, "unix:nlink"));

    // A FileStore is the storage that holds a path.
    FileStore store = Files.getFileStore(main);
    System.out.println("usable space > 0: " + (store.getUsableSpace() > 0));
    System.out.println("separator: " + FileSystems.getDefault().getSeparator());

    // A zip file is a file system too. The same Path and Files calls work in it.
    URI zip = URI.create("jar:" + Path.of("tree.zip").toAbsolutePath().toUri());
    try (FileSystem zfs = FileSystems.newFileSystem(zip, Map.of("create", "true"))) {
      Path inZip = zfs.getPath("/docs/notes.txt");
      Files.createDirectories(inZip.getParent());
      Files.writeString(inZip, "inside a zip\n");
      System.out.print("read back: " + Files.readString(inZip));
      System.out.println("zip separator: " + zfs.getSeparator());
    }
    System.out.println("zip exists: " + Files.isRegularFile(Path.of("tree.zip")));
  }
}
```

```output for=zip-file-system
same file: true
links: 2
usable space > 0: true
separator: /
read back: inside a zip
zip separator: /
zip exists: true
```

- Verdict: stdlib. `Files.isSameFile` tells whether two paths name the same file, and the `unix` attribute view gives the link count. `Files.getFileStore` gives the storage that holds a path. A zip archive is also a `FileSystem`: `FileSystems.newFileSystem` opens it, and the same `Files.writeString` and `Files.readString` calls work inside it.
