```java run id=leap
import java.util.LinkedHashMap;
import java.util.Map;

public class Main {
    static boolean isLeap(int year) {
        return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
    }

    static void check(boolean condition, String message) {
        if (!condition) {
            throw new AssertionError(message);
        }
    }

    public static void main(String[] args) {
        // The tests in the order of the to-do list. Each one added one clause.
        Map<String, Runnable> tests = new LinkedHashMap<>();
        tests.put("2024 is a leap year", () -> check(isLeap(2024), "2024"));
        tests.put("2023 is a common year", () -> check(!isLeap(2023), "2023"));
        tests.put("1900 is a common year", () -> check(!isLeap(1900), "1900"));
        tests.put("2000 is a leap year", () -> check(isLeap(2000), "2000"));

        int failed = 0;
        for (var test : tests.entrySet()) {
            try {
                test.getValue().run();
                System.out.println("pass  " + test.getKey());
            } catch (AssertionError e) {
                failed++;
                System.out.println("FAIL  " + test.getKey() + ": " + e.getMessage());
            }
        }
        System.out.println(failed == 0 ? "green" : "red");
    }
}
```

```output
pass  2024 is a leap year
pass  2023 is a common year
pass  1900 is a common year
pass  2000 is a leap year
green
```

- Verdict: library. The usual runner is JUnit, which is not part of the JDK. This example uses a small runner in `main`, so that it runs with the standard library only. The structure is the same: a name and a body for each test, and a report.
- `LinkedHashMap` keeps the order of insertion, so the tests run in the order of the to-do list.
- `check` throws `AssertionError` itself. It does not use the `assert` statement; the next block shows why.

A test with the `assert` statement can pass with wrong code. The `java` launcher disables assertions unless it gets the option `-ea`, so the `assert` statements below never run.

```java wrong id=assert-off
public class Main {
    static boolean isLeap(int year) {
        return year % 4 == 0; // bug: 1900 is not a leap year
    }

    public static void main(String[] args) {
        assert isLeap(2024);
        assert !isLeap(2023);
        assert !isLeap(1900); // must fail, but assertions are off
        System.out.println("all tests passed");
    }
}
```

```output for=assert-off
all tests passed
```

- The cycle catches this mistake. The test for 1900 must be red before the fix, and here it is never red. A test that has not failed once has not shown that it runs.
- JUnit has its own assertion methods, such as `assertTrue`, which throw whether or not `-ea` is set.

The first red in Java is a compile error, because `isLeap` does not exist yet.

```java fails expect=compile id=red
public class Main {
    public static void main(String[] args) {
        // Red: the test comes first, so isLeap does not exist yet.
        if (!isLeap(2024)) {
            throw new AssertionError("2024");
        }
    }
}
```

- `javac` reports "cannot find symbol". To move on, write a stub with the right signature, such as `static boolean isLeap(int year) { return false; }`. Then the program compiles and the test fails on its check.
