```java wrong id=record-tostring
public class Main {
    record Login(String user, String password) {}

    public static void main(String[] args) {
        // A placeholder password, as a user would type it.
        Login login = new Login("ana", "PLACEHOLDER-PASSWORD");
        // A debug line written while the request is handled.
        System.out.println("debug: login request " + login);
    }
}
```

```output for=record-tostring
debug: login request Login[user=ana, password=PLACEHOLDER-PASSWORD]
```

The implicit `toString` of a record contains the names and the values of all its components, so the password goes into the debug line.

```java run id=override-tostring
public class Main {
    record Login(String user, char[] password) {
        // The generated toString shows every component, so the record replaces it.
        @Override
        public String toString() {
            return "Login[user=" + user + ", password=***]";
        }
    }

    public static void main(String[] args) {
        // A placeholder password, as a user would type it.
        Login login = new Login("ana", "PLACEHOLDER-PASSWORD".toCharArray());
        System.out.println("debug: login request " + login);
        // The code that checks the password reads the component by name.
        System.out.println("password length: " + login.password().length);
        java.util.Arrays.fill(login.password(), '\0'); // clear it after use
    }
}
```

```output for=override-tostring
debug: login request Login[user=ana, password=***]
password length: 20
```

The record overrides `toString` and prints `***` for the password. The password is a `char[]`, so the code can clear it after use.

- Verdict: stdlib. A record can declare its own `toString`, and the compiler then does not generate one.
- The documentation of `java.io.Console.readPassword` returns a `char[]` and advises to zero the array after use. A `String` cannot be cleared.
- A record with a `char[]` component uses the identity of the array in `equals` and `hashCode`. That is fine for a request object, but not for a value that the program compares.
- Logging frameworks call `toString` on the objects that they receive. An overridden `toString` protects every log call at once.
