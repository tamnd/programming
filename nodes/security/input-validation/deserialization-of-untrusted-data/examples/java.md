```java wrong id=cast-too-late
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;

public class Main {
    record Settings(String theme) implements Serializable {}

    // A class that the program can load, with code that runs on load.
    static class Audit implements Serializable {
        private void readObject(ObjectInputStream in) throws IOException, ClassNotFoundException {
            in.defaultReadObject();
            System.out.println("Audit.readObject ran");
        }
    }

    static byte[] save(Object value) throws IOException {
        var bytes = new ByteArrayOutputStream();
        try (var out = new ObjectOutputStream(bytes)) {
            out.writeObject(value);
        }
        return bytes.toByteArray();
    }

    static Settings load(byte[] data) throws IOException, ClassNotFoundException {
        try (var in = new ObjectInputStream(new ByteArrayInputStream(data))) {
            return (Settings) in.readObject();
        }
    }

    public static void main(String[] args) throws Exception {
        System.out.println(load(save(new Settings("dark"))));
        try {
            load(save(new Audit()));
        } catch (ClassCastException e) {
            System.out.println("ClassCastException, after the object was built");
        }
    }
}
```

```output for=cast-too-late
Settings[theme=dark]
Audit.readObject ran
ClassCastException, after the object was built
```

The cast to `Settings` looks like a type check. `readObject` builds the object first, and the code of its class runs during the build. The cast fails after that code has run.

```java run id=filter-first
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InvalidClassException;
import java.io.ObjectInputFilter;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;

public class Main {
    record Settings(String theme) implements Serializable {}

    static class Audit implements Serializable {
        private void readObject(ObjectInputStream in) throws IOException, ClassNotFoundException {
            in.defaultReadObject();
            System.out.println("Audit.readObject ran");
        }
    }

    static byte[] save(Object value) throws IOException {
        var bytes = new ByteArrayOutputStream();
        try (var out = new ObjectOutputStream(bytes)) {
            out.writeObject(value);
        }
        return bytes.toByteArray();
    }

    static Settings load(byte[] data) throws IOException, ClassNotFoundException {
        try (var in = new ObjectInputStream(new ByteArrayInputStream(data))) {
            in.setObjectInputFilter(ObjectInputFilter.Config.createFilter("Main$Settings;!*"));
            return (Settings) in.readObject();
        }
    }

    public static void main(String[] args) throws Exception {
        System.out.println(load(save(new Settings("dark"))));
        try {
            load(save(new Audit()));
        } catch (InvalidClassException e) {
            System.out.println("refused: " + e.getMessage());
        }
    }
}
```

```output for=filter-first
Settings[theme=dark]
refused: filter status: REJECTED
```

The filter sees the class `Main$Audit` before the object exists and rejects it, so `Audit.readObject` does not run.

- Verdict: stdlib. `java.io` serialization can create any serializable class on the class path. `ObjectInputFilter` filters classes, array sizes and the depth of the stream.
- Prefer a pure data format, and parse it into records that hold only the needed fields.
- If a program must read Java object streams, set a filter on each stream or a JVM-wide filter with `jdk.serialFilter`, and end the pattern with `!*`.
- The OWASP cheat sheet shows a `final` `readObject` that always throws, for a class that must never be deserialized.
