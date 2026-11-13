```java run id=posix-file-permissions
import java.nio.file.*;
import java.nio.file.attribute.*;
import java.util.Set;

public class Main {
    public static void main(String[] args) throws Exception {
        Path p = Files.writeString(Path.of("data.txt"), "x");

        // A permission set is a Set of enum values.
        Set<PosixFilePermission> perms = PosixFilePermissions.fromString("rw-r-----");
        Files.setPosixFilePermissions(p, perms);
        System.out.println("set: " + PosixFilePermissions.toString(Files.getPosixFilePermissions(p)));

        // Add one permission, as chmod u+x does.
        Set<PosixFilePermission> now = Files.getPosixFilePermissions(p);
        now.add(PosixFilePermission.OWNER_EXECUTE);
        Files.setPosixFilePermissions(p, now);
        System.out.println("after u+x: " + PosixFilePermissions.toString(Files.getPosixFilePermissions(p)));
        System.out.println("contains GROUP_WRITE: " + now.contains(PosixFilePermission.GROUP_WRITE));

        // The unix view gives the mode as a number, with the type bits.
        int mode = (Integer) Files.getAttribute(p, "unix:mode");
        System.out.println("unix:mode & 07777 = " + Integer.toOctalString(mode & 07777));

        // The views that the file system supports.
        Set<String> views = p.getFileSystem().supportedFileAttributeViews();
        System.out.println("posix view: " + views.contains("posix") + ", acl view: " + views.contains("acl"));
    }
}
```

```output for=posix-file-permissions
set: rw-r-----
after u+x: rwxr-----
contains GROUP_WRITE: false
unix:mode & 07777 = 740
posix view: true, acl view: false
```

- Verdict: stdlib. `PosixFilePermission` is an enum with nine values, such as `OWNER_READ` and `OTHERS_EXECUTE`, and a set of them is the permission bits of a file. `PosixFilePermissions.fromString` and `toString` use the form of `ls`. The enum has no setuid, setgid or sticky bit, but the `unix:mode` attribute gives the whole mode as a number. On Windows, the file system has the `acl` view and not the `posix` view.
