```java run id=syntax-and-type-errors
import com.sun.source.util.JavacTask;
import java.net.URI;
import java.util.List;
import javax.tools.Diagnostic;
import javax.tools.DiagnosticCollector;
import javax.tools.JavaFileObject;
import javax.tools.SimpleJavaFileObject;
import javax.tools.ToolProvider;

public class Main {
    static String check(String code) {
        var compiler = ToolProvider.getSystemJavaCompiler();
        var diags = new DiagnosticCollector<JavaFileObject>();
        var src = new SimpleJavaFileObject(URI.create("string:///T.java"), JavaFileObject.Kind.SOURCE) {
            @Override public CharSequence getCharContent(boolean ignore) { return code; }
        };
        var task = (JavacTask) compiler.getTask(null, null, diags, List.of("-proc:none"), null, List.of(src));
        try {
            task.analyze();  // parse and check types, but write no class file
        } catch (java.io.IOException e) {
            return e.toString();
        }
        if (diags.getDiagnostics().isEmpty()) return "ok";
        Diagnostic<?> d = diags.getDiagnostics().get(0);
        return "line " + d.getLineNumber() + ": " + d.getMessage(null);
    }

    public static void main(String[] args) {
        // A syntax error: the parser expects a semicolon.
        System.out.println(check("class T { int x = 1 }"));
        // Correct syntax, but a type error: the compiler finds it after the parse.
        System.out.println(check("class T { int x = \"one\"; }"));
        System.out.println(check("class T { int x = 1; }"));

        // The enhanced for statement is sugar for a loop with an Iterator.
        int sum = 0;
        for (int v : List.of(1, 2, 3)) sum += v;
        var it = List.of(1, 2, 3).iterator();
        int sum2 = 0;
        while (it.hasNext()) sum2 += it.next();
        System.out.println(sum + " " + sum2);
    }
}
```

```output for=syntax-and-type-errors
line 1: ';' expected
line 1: incompatible types: java.lang.String cannot be converted to int
ok
6 6
```

- Verdict: native. The compiler API compiles a string and reports diagnostics. A missing `;` is a syntax error. `int x = "one";` has valid syntax, and the compiler rejects it after the parse because the types do not match. The enhanced `for` statement is sugar for a loop with an `Iterator`, so the two sums are equal.
