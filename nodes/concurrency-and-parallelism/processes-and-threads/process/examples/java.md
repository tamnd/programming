```java run id=child
import java.io.IOException;

public class Main {
    static final String SCRIPT =
        "echo \"child sees GREETING=$GREETING\"; GREETING=changed; echo \"child set GREETING=$GREETING\"; exit 3";

    public static void main(String[] args) throws IOException, InterruptedException {
        String greeting = "hello";
        ProcessBuilder builder = new ProcessBuilder("sh", "-c", SCRIPT);
        builder.environment().put("GREETING", greeting);
        Process child = builder.start();
        String out = new String(child.getInputStream().readAllBytes());
        int status = child.waitFor();
        System.out.print(out);
        System.out.println("child exit status: " + status);
        System.out.println("child pid differs: " + (child.pid() != ProcessHandle.current().pid()));
        System.out.println("parent greeting: " + greeting);
    }
}
```

```output for=child
child sees GREETING=hello
child set GREETING=changed
child exit status: 3
child pid differs: true
parent greeting: hello
```

- Verdict: stdlib. `ProcessBuilder` holds the command, the environment and the directory; `start()` returns a `Process`.
- `environment()` is a copy of the environment of the JVM, so `put` changes only what the child gets.
- The program reads all the output before `waitFor()`. A child that fills the pipe while the parent waits without reading can block both.
- `ProcessHandle.current()` describes the JVM process itself, and `Process.pid()` gives the ID of the child.
