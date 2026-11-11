```java run id=jobs
public class Main {
    static int runJob(int job, int[] items) {
        try {
            return items[job] * 10;
        } finally {
            System.out.println("cleanup job " + job);
        }
    }

    public static void main(String[] args) {
        int[] items = {1, 2};
        for (int job : new int[] {0, 5, 1}) {
            try {
                System.out.println("job " + job + " -> " + runJob(job, items));
            } catch (Exception e) {
                System.out.println("job " + job + " failed: " + e);
            }
        }

        Throwable bug = new AssertionError("broken invariant");
        System.out.println("AssertionError is an Exception: " + (bug instanceof Exception));
        System.out.println("AssertionError is an Error: " + (bug instanceof Error));
    }
}
```

```output
cleanup job 0
job 0 -> 10
cleanup job 5
job 5 failed: java.lang.ArrayIndexOutOfBoundsException: Index 5 out of bounds for length 2
cleanup job 1
job 1 -> 20
AssertionError is an Exception: false
AssertionError is an Error: true
```

- Verdict: idiom. Java has no panic, but it splits `Throwable` in two. An `Exception` is a condition that a reasonable application might want to catch. An `Error` is a serious problem that it should not try to catch.
- `ArrayIndexOutOfBoundsException` is a `RuntimeException`, so the boundary above catches it. `AssertionError`, `OutOfMemoryError` and `StackOverflowError` are subclasses of `Error`.
- `System.exit` starts the shutdown sequence and runs the shutdown hooks. `Runtime.halt` ends the JVM at once, without the hooks.

A handler for `Exception` does not catch an `Error`. The `AssertionError` passes the boundary and ends the program.

```java fails expect=exception:AssertionError id=error-passes
public class Main {
    static void runJob(int job) {
        if (job < 0) {
            throw new AssertionError("negative job " + job);
        }
        System.out.println("job " + job + " done");
    }

    public static void main(String[] args) {
        for (int job : new int[] {1, -1, 2}) {
            try {
                runJob(job);
            } catch (Exception e) {
                System.out.println("job " + job + " failed: " + e);
            }
        }
    }
}
```

When the JVM halts, the threads stop running Java code. The `finally` block does not run.

```java fails expect=exit:3 id=halt
public class Main {
    public static void main(String[] args) {
        Runtime.getRuntime().addShutdownHook(new Thread(() -> System.out.println("shutdown hook")));
        try {
            Runtime.getRuntime().halt(3);
        } finally {
            System.out.println("cleanup");
        }
    }
}
```
