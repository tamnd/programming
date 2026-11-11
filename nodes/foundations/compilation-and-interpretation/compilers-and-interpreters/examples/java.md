```java run id=compiler-as-a-library
import java.lang.reflect.Method;
import java.net.URLClassLoader;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Comparator;
import javax.tools.ToolProvider;

public class Main {
    public static void main(String[] args) throws Exception {
        Path dir = Files.createTempDirectory("compile-demo");
        Path src = dir.resolve("Formula.java");
        Files.writeString(src, "public class Formula { public static int f(int x) { return x * 2 + 1; } }");

        var javac = ToolProvider.getSystemJavaCompiler(); // the compiler as a library
        int status = javac.run(null, null, null, src.toString());
        Path cls = dir.resolve("Formula.class");
        byte[] bytes = Files.readAllBytes(cls);
        System.out.printf("%d %X%n", status, ((bytes[0] & 0xff) << 24) | ((bytes[1] & 0xff) << 16)
                | ((bytes[2] & 0xff) << 8) | (bytes[3] & 0xff)); // every class file starts with CAFEBABE

        try (var loader = new URLClassLoader(new java.net.URL[] {dir.toUri().toURL()})) {
            Method f = loader.loadClass("Formula").getMethod("f", int.class); // the JVM loads the bytecode
            System.out.println(f.invoke(null, 20));
        }

        try (var paths = Files.walk(dir)) {
            paths.sorted(Comparator.reverseOrder()).forEach(p -> p.toFile().delete());
        }
    }
}
```

```output for=compiler-as-a-library
0 CAFEBABE
41
```

- Verdict: native. `javac` compiles Java source to class files that hold bytecode for the JVM, and every class file starts with the bytes `CAFEBABE`. The JVM loads classes when the program first uses them, interprets their bytecode, and compiles hot methods to machine code with its JIT compilers.
- The example uses the compiler as a library through `javax.tools`, loads the new class with a class loader, and calls its method through reflection. The command `java Main.java` does the same for a single source file: it compiles the file in memory and then runs it.
