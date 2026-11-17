```java run id=trace
import java.util.stream.Collectors;

public class Main {
    static String where() {
        return StackWalker.getInstance().walk(frames -> frames
                .skip(1) // skip where itself
                .map(StackWalker.StackFrame::getMethodName)
                .collect(Collectors.joining(" <- ")));
    }

    static void c() {
        System.out.println("      stack: " + where());
    }

    static void b() {
        System.out.println("    call c");
        c();
        System.out.println("    back in b");
    }

    static void a() {
        System.out.println("  call b");
        b();
        System.out.println("  back in a");
    }

    public static void main(String[] args) {
        System.out.println("call a");
        a();
        System.out.println("back in main");
    }
}
```

```output for=trace
call a
  call b
    call c
      stack: c <- b <- a <- main
    back in b
  back in a
back in main
```

- Verdict: native. Each thread has its own JVM stack. `StackWalker` gives the frames of the current thread as a stream, from the top frame down.
- `Throwable.getStackTrace()` gives the same data for the place where an exception was made.
