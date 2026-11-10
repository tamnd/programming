```java wrong id=sh-c
import java.io.IOException;

public class Main {
    static void lookUp(String name) throws IOException, InterruptedException {
        new ProcessBuilder("sh", "-c", "echo looking up " + name).inheritIO().start().waitFor();
    }

    public static void main(String[] args) throws Exception {
        lookUp("report.txt");
        lookUp("report.txt; echo INJECTED");
    }
}
```

```output for=sh-c
looking up report.txt
looking up report.txt
INJECTED
```

```java run id=args
import java.io.IOException;

public class Main {
    static void lookUp(String name) throws IOException, InterruptedException {
        new ProcessBuilder("echo", "looking up", name).inheritIO().start().waitFor();
    }

    public static void main(String[] args) throws Exception {
        lookUp("report.txt");
        lookUp("report.txt; echo INJECTED");
    }
}
```

```output for=args
looking up report.txt
looking up report.txt; echo INJECTED
```

- Verdict: stdlib. `ProcessBuilder` takes the program and its arguments as separate strings and starts no shell.
- `Runtime.exec(String)` does not start a shell either. It splits the string at white space, so a value with a space becomes more than one argument. This is argument injection. The OWASP cheat sheet says to use `ProcessBuilder` or `Runtime.exec(String[])`.
- The weakness comes back with `sh -c` or `cmd /c` and a string built from input, as in the first example.
- CodeQL query `java/command-line-injection` follows input from a request to `ProcessBuilder` and `Runtime.exec`.
