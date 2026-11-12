```java run id=object-streams-and-filters
import java.io.*;
import java.time.Instant;
import java.util.*;

public class Main {
    record Person(String name, int born, List<String> tags) implements Serializable {}

    static class Session implements Serializable {
        private static final long serialVersionUID = 1L;
        String user = "ada";
        transient String password = "secret"; // not written
    }

    static byte[] write(Object o) throws IOException {
        var bytes = new ByteArrayOutputStream();
        try (var out = new ObjectOutputStream(bytes)) {
            out.writeObject(o);
        }
        return bytes.toByteArray();
    }

    static Object read(byte[] data) throws IOException, ClassNotFoundException {
        try (var in = new ObjectInputStream(new ByteArrayInputStream(data))) {
            // accept only the classes this program expects
            in.setObjectInputFilter(ObjectInputFilter.Config.createFilter(
                "Main$*;java.util.*;java.lang.*;!*"));
            return in.readObject();
        }
    }

    public static void main(String[] args) throws Exception {
        var ada = new Person("Ada", 1815, new ArrayList<>(List.of("math")));
        var back = (Person) read(write(ada));
        System.out.println(back + " equal: " + back.equals(ada));

        var session = (Session) read(write(new Session()));
        System.out.println("user " + session.user + ", password " + session.password);

        var shared = new ArrayList<>(List.of("x"));
        var pair = (List<?>) read(write(new ArrayList<>(List.of(shared, shared))));
        System.out.println("sharing kept: " + (pair.get(0) == pair.get(1)));

        try {
            write(new Object());
        } catch (NotSerializableException e) {
            System.out.println("NotSerializableException: " + e.getMessage());
        }
        try {
            read(write(Instant.EPOCH));
        } catch (InvalidClassException e) {
            System.out.println("filter rejected: " + e.getMessage());
        }
    }
}
```

```output for=object-streams-and-filters
Person[name=Ada, born=1815, tags=[math]] equal: true
user ada, password null
sharing kept: true
NotSerializableException: java.lang.Object
filter rejected: filter status: REJECTED
```

- Verdict: stdlib. A class that implements `java.io.Serializable` can go through `ObjectOutputStream` and `ObjectInputStream`, which keep shared references and skip `transient` fields. Records are serialized through their canonical constructor. The format names the classes to create, so reading untrusted bytes is dangerous; the example sets an `ObjectInputFilter` that accepts only the expected classes and rejects `Instant`. For data that other programs read, Java programs use JSON libraries such as Jackson.
