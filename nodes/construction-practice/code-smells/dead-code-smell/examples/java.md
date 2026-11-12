```java run id=delete-what-does-not-run
import java.net.URI;
import java.util.List;
import javax.tools.Diagnostic;
import javax.tools.DiagnosticCollector;
import javax.tools.JavaCompiler;
import javax.tools.JavaFileObject;
import javax.tools.SimpleJavaFileObject;
import javax.tools.ToolProvider;

public class Main {
    static final boolean LEGACY_USD = false;

    // Before: a branch behind a constant that is always false. The compiler
    // allows this form of dead code, on purpose, for conditional compilation.
    static long priceBefore(long cents) {
        long withTax = Math.round(cents * 1.2);
        if (LEGACY_USD) {
            return withTax + 30;
        }
        return withTax;
    }

    // After: only the code that runs.
    static long price(long cents) {
        return Math.round(cents * 1.2);
    }

    // Compile a source string in memory and return the error messages.
    static List<String> errors(String source) {
        JavaCompiler javac = ToolProvider.getSystemJavaCompiler();
        JavaFileObject file = new SimpleJavaFileObject(URI.create("string:///Probe.java"), JavaFileObject.Kind.SOURCE) {
            @Override
            public CharSequence getCharContent(boolean ignoreEncodingErrors) {
                return source;
            }
        };
        DiagnosticCollector<JavaFileObject> diagnostics = new DiagnosticCollector<>();
        javac.getTask(null, null, diagnostics, List.of("-d", System.getProperty("java.io.tmpdir")), null, List.of(file)).call();
        return diagnostics.getDiagnostics().stream()
                .filter(d -> d.getKind() == Diagnostic.Kind.ERROR)
                .map(d -> d.getMessage(null))
                .toList();
    }

    public static void main(String[] args) {
        for (long cents : new long[] {0, 1, 999, 1250}) {
            check(price(cents) == priceBefore(cents), "same price for " + cents);
        }

        // A statement after return is a compile-time error.
        List<String> afterReturn = errors("class Probe { int f() { return 1; System.out.println(); } }");
        check(afterReturn.stream().anyMatch(m -> m.contains("unreachable statement")), "after return: " + afterReturn);

        // A while (false) body is also refused, but if (false) is allowed.
        check(!errors("class Probe { void f() { while (false) { System.out.println(); } } }").isEmpty(), "while false");
        check(errors("class Probe { void f() { if (false) { System.out.println(); } } }").isEmpty(), "if false");
        System.out.println("price: tests pass");
    }

    static void check(boolean ok, String what) {
        if (!ok) {
            throw new AssertionError(what);
        }
    }
}
```

```output
price: tests pass
```

The Java Language Specification makes a statement that cannot be reached a compile-time error, and the probe shows `javac` refusing a statement after `return` and the body of `while (false)`. The rule for `if` is different by design: the specification allows `if (false)` so that a constant flag such as `LEGACY_USD` can switch code off. That code is dead, and the compiler does not report it, so a linter or a review must.
