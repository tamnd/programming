```java run id=attribute-views
import java.nio.file.*;
import java.nio.file.attribute.*;
import java.time.Instant;

public class Main {
    public static void main(String[] args) throws Exception {
        Path data = Path.of("data.txt");
        Files.writeString(data, "hello");
        Files.setPosixFilePermissions(data, PosixFilePermissions.fromString("rw-r-----"));
        Files.setLastModifiedTime(data, FileTime.from(Instant.ofEpochSecond(1700000000)));
        Files.createLink(Path.of("second-name.txt"), data);
        Path link = Files.createSymbolicLink(Path.of("link"), data);

        // readAttributes follows the link, unless NOFOLLOW_LINKS is given.
        PosixFileAttributes st = Files.readAttributes(link, PosixFileAttributes.class);
        BasicFileAttributes lst = Files.readAttributes(link, BasicFileAttributes.class,
                LinkOption.NOFOLLOW_LINKS);
        System.out.println("stat:  regular " + st.isRegularFile() + ", size " + st.size()
                + ", " + PosixFilePermissions.toString(st.permissions())
                + ", " + st.lastModifiedTime());
        System.out.println("lstat: symlink " + lst.isSymbolicLink() + ", size " + lst.size());

        // The unix view gives the fields that have no portable name.
        System.out.println("links: " + Files.getAttribute(data, "unix:nlink"));
        Object key = Files.readAttributes(Path.of("second-name.txt"),
                BasicFileAttributes.class).fileKey();
        System.out.println("same fileKey: " + key.equals(st.fileKey()));
    }
}
```

```output for=attribute-views
stat:  regular true, size 5, rw-r-----, 2023-11-14T22:13:20Z
lstat: symlink true, size 8
links: 2
same fileKey: true
```

- Verdict: stdlib. `Files.readAttributes` reads a set of attributes in one operation. The interface that the program asks for selects the view: `BasicFileAttributes` has the portable attributes, and `PosixFileAttributes` adds the owner, the group and the permissions. `LinkOption.NOFOLLOW_LINKS` reads the link itself. A string such as `"unix:nlink"` names one attribute in a view, and `fileKey` identifies the file, so two hard links give equal keys.
