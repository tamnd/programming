```java run id=stackwalker
import java.lang.management.ManagementFactory;
import java.util.List;

public class Main {
    // The frames of this class on the stack, innermost first: method and line.
    static List<String> whereAmI() {
        return StackWalker.getInstance().walk(frames -> frames
            .skip(1) // skip whereAmI itself
            .filter(f -> f.getClassName().equals("Main"))
            .map(f -> f.getMethodName() + " line " + f.getLineNumber())
            .toList());
    }

    static int price(String item) {
        if (item.equals("bread")) {
            System.out.println("  " + String.join(" <- ", whereAmI()));
            return 250;
        }
        return 30;
    }

    static int total(List<String> cart) {
        int sum = 0;
        for (String item : cart) {
            sum += price(item);
        }
        return sum;
    }

    public static void main(String[] args) {
        System.out.println("total: " + total(List.of("apple", "bread")));

        // A debugger connects through the JDWP agent, which the JVM loads
        // with an option such as -agentlib:jdwp=transport=dt_socket,server=y
        boolean agent = ManagementFactory.getRuntimeMXBean().getInputArguments().stream()
            .anyMatch(arg -> arg.startsWith("-agentlib:jdwp"));
        System.out.println("JDWP agent loaded: " + agent);
    }
}
```

```output for=stackwalker
  price line 16 <- total line 25 <- main line 31
total: 280
JDWP agent loaded: false
```

A `StackWalker` made without options does not keep the `Class` object of each frame. It keeps only the names.

```java fails expect=exception:UnsupportedOperationException id=class-ref
public class Main {
    public static void main(String[] args) {
        // Without the option RETAIN_CLASS_REFERENCE, a StackFrame does not
        // keep the Class object, and getDeclaringClass throws.
        StackWalker walker = StackWalker.getInstance();
        walker.forEach(frame -> System.out.println(frame.getDeclaringClass().getName()));
    }
}
```

The fix is `StackWalker.getInstance(StackWalker.Option.RETAIN_CLASS_REFERENCE)`, or `getClassName()` when the name is enough.

- Verdict: stdlib. The JDK ships jdb, a command-line debugger, and the Java Platform Debugger Architecture that IDE debuggers use.
- `java -agentlib:jdwp=transport=dt_socket,server=y,suspend=n Main` starts a program that a debugger can attach to. `jdb -attach 8000` attaches to the port.
