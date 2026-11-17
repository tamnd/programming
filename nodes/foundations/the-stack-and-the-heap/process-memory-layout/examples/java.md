```java run id=regions
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

public class Main {
    public static void main(String[] args) throws Exception {
        Map<String, Long> found = new LinkedHashMap<>();
        for (String line : Files.readAllLines(Path.of("/proc/self/maps"))) {
            String[] fields = line.trim().split("\\s+");
            String path = fields.length > 5 ? fields[5] : "";
            if (List.of("[heap]", "[stack]", "[vdso]").contains(path) && !found.containsKey(path)) {
                found.put(path, Long.parseUnsignedLong(fields[0].split("-")[0], 16));
                System.out.println(path + " " + fields[1]);
            }
        }
        boolean below = Long.compareUnsigned(found.get("[heap]"), found.get("[stack]")) < 0;
        System.out.println("the heap is below the stack: " + below);
    }
}
```

```output for=regions
[heap] rw-p
[stack] rw-p
[vdso] r-xp
the heap is below the stack: true
```

- Verdict: native. Java has no addresses, but the JVM runs in a process with the usual layout, and the program can read `/proc/self/maps` as a file.
