```java run id=jfr
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.List;
import jdk.jfr.Configuration;
import jdk.jfr.Recording;
import jdk.jfr.consumer.RecordedEvent;
import jdk.jfr.consumer.RecordingFile;

public class Main {
    static long fibCalls = 0;

    static int fib(int n) {
        fibCalls++;
        return n < 2 ? n : fib(n - 1) + fib(n - 2);
    }

    public static void main(String[] args) throws Exception {
        // The JDK ships two settings files for Flight Recorder.
        for (String name : List.of("default", "profile")) {
            Configuration config = Configuration.getConfiguration(name);
            System.out.println(name + ": jdk.ExecutionSample every "
                + config.getSettings().get("jdk.ExecutionSample#period"));
        }

        Path file = Files.createTempFile("fib", ".jfr");
        try (Recording recording = new Recording(Configuration.getConfiguration("profile"))) {
            recording.start();
            System.out.println("fib(25) = " + fib(25));
            recording.stop();
            recording.dump(file);
        }
        List<RecordedEvent> events = RecordingFile.readAllEvents(file);
        Files.delete(file);
        // The number of samples depends on timing, so print only whether there are events.
        System.out.println("recording has events: " + !events.isEmpty());
        System.out.println("fib calls: " + fibCalls);
    }
}
```

```output for=jfr
default: jdk.ExecutionSample every 20 ms
profile: jdk.ExecutionSample every 10 ms
fib(25) = 75025
recording has events: true
fib calls: 242785
```

- Verdict: stdlib. JDK Flight Recorder is in the JDK. The `jdk.jfr` API starts and stops a recording from the program, and `-XX:StartFlightRecording` starts one from the command line.
- The event `jdk.ExecutionSample` is a snapshot of a thread that executes Java code. The two settings files of the JDK, `default` and `profile`, sample at different periods.
- The number of samples depends on timing, so the example prints only whether there are events. The call counter is manual instrumentation.
