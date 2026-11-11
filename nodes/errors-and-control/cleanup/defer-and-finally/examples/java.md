```java run id=copy
public class Main {
    static class File implements AutoCloseable {
        final String name;
        final boolean closeFails;

        File(String name, boolean closeFails) {
            this.name = name;
            this.closeFails = closeFails;
            System.out.println("open " + name);
        }

        @Override
        public void close() {
            System.out.println("close " + name);
            if (closeFails) throw new IllegalStateException("close failed: " + name);
        }
    }

    static void copy(String src, String dst, boolean fail) {
        try (File a = new File(src, false); File b = new File(dst, fail)) {
            if (fail) throw new IllegalStateException("cannot read " + a.name);
            System.out.println("copy " + a.name + " to " + b.name);
        }
    }

    static String label() {
        String text = "try";
        try {
            return text;
        } finally {
            text = "finally";
            System.out.println("finally ran; text is now " + text);
        }
    }

    public static void main(String[] args) {
        copy("a.txt", "b.txt", false);
        try {
            copy("a.txt", "c.txt", true);
        } catch (IllegalStateException e) {
            System.out.println("caught: " + e.getMessage());
            for (Throwable s : e.getSuppressed()) {
                System.out.println("suppressed: " + s.getMessage());
            }
        }
        System.out.println("label() returned " + label());
    }
}
```

```output
open a.txt
open b.txt
copy a.txt to b.txt
close b.txt
close a.txt
open a.txt
open c.txt
close c.txt
close a.txt
caught: cannot read a.txt
suppressed: close failed: c.txt
finally ran; text is now finally
label() returned try
```

- Verdict: native. `try` with `finally` runs on every exit. A try-with-resources statement closes each `AutoCloseable` resource in reverse order of initialization.
- When the body throws and `close` also throws, the statement keeps the exception from the body and adds the one from `close` as a suppressed exception.
- The `return` in `label` computes its value before `finally` runs, so the assignment in `finally` does not change the result.

With a plain `finally`, an exception from `close` replaces the exception from the body. The caller sees only the second failure, and the first one is lost.

```java wrong id=finally-close
public class Main {
    static void close(String name) {
        throw new IllegalStateException("close failed: " + name);
    }

    static void copy(String src) {
        try {
            throw new IllegalStateException("cannot read " + src);
        } finally {
            close(src);
        }
    }

    public static void main(String[] args) {
        try {
            copy("a.txt");
        } catch (IllegalStateException e) {
            System.out.println("caught: " + e.getMessage());
            System.out.println("suppressed: " + e.getSuppressed().length);
        }
    }
}
```

```output
caught: close failed: a.txt
suppressed: 0
```

A `return` in `finally` discards the exception of the `try` block. `javac` compiles this code without an error.

```java wrong id=finally-return
public class Main {
    static String save(String record) {
        try {
            if (record.isEmpty()) throw new IllegalArgumentException("empty record");
            return "saved";
        } finally {
            return "done";
        }
    }

    public static void main(String[] args) {
        System.out.println(save("id=1"));
        System.out.println(save(""));
    }
}
```

```output
done
done
```
