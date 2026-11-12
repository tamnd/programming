```java run id=compile-from-source
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.HexFormat;
import javax.tools.ToolProvider;

public class Main {
    public static void main(String[] args) throws Exception {
        // The program writes source text to a file and calls the compiler on it.
        Path dir = Files.createTempDirectory("src");
        Path src = dir.resolve("Hello.java");
        Files.writeString(src, "public class Hello { static int answer() { return 6 * 7; } }\n");
        int rc = ToolProvider.getSystemJavaCompiler().run(null, null, null, src.toString());
        System.out.println("compiler exit code: " + rc);

        // The compiler writes a class file of bytecode, which starts with the bytes CAFEBABE.
        byte[] cls = Files.readAllBytes(dir.resolve("Hello.class"));
        System.out.println("class file starts with: " + HexFormat.of().formatHex(cls, 0, 4));

        // A stack trace maps running code back to a file and a line of source.
        StackTraceElement here = new Throwable().getStackTrace()[0];
        System.out.println("this line: " + here.getFileName() + ":" + here.getLineNumber());
        for (Path p : new Path[] {dir.resolve("Hello.class"), src, dir}) Files.delete(p);
    }
}
```

```output for=compile-from-source
compiler exit code: 0
class file starts with: cafebabe
this line: Main.java:20
```

- Verdict: native. The program writes a compilation unit to a file and calls the system Java compiler through `javax.tools`, which returns 0 when the compile succeeds. The output is a class file of bytecode that starts with the magic number `CAFEBABE`. The compiler also writes a line number table, so a stack trace names the source file and the line of the running code.
