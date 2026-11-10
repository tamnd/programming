```java fails expect=exception:IOException id=closed-reader
import java.io.BufferedReader;
import java.io.IOException;
import java.io.StringReader;

public class Main {
    public static void main(String[] args) throws IOException {
        BufferedReader reader = new BufferedReader(new StringReader("alice\nbob\n"));
        System.out.println(reader.readLine());
        reader.close();
        System.out.println(reader.readLine());
    }
}
```

```java run id=try-with-resources
import java.io.BufferedReader;
import java.io.IOException;
import java.io.StringReader;

public class Main {
    public static void main(String[] args) throws IOException {
        try (BufferedReader reader = new BufferedReader(new StringReader("alice\nbob\n"))) {
            System.out.println(reader.readLine());
            System.out.println(reader.readLine());
        }
        // reader is not in scope here, so no code can use it after the close.
    }
}
```

```output for=try-with-resources
alice
bob
```

- Verdict: native. The garbage collector keeps each object alive while a reference to it exists, so Java code cannot use freed memory.
- The bug moves to resources. The documentation of `Reader.close` says that after the close, further calls to `read` and some other methods throw `IOException`. The error is safe and visible.
- `try`-with-resources closes the resource at the end of the block, and the variable is not in scope after it.
- Native code through JNI can free memory that Java still points to. Such code has the same weakness as C.
