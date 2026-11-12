```java fails expect=exception:StackOverflowError id=deep-recursion
public class Main {
    // Parse nested brackets with one call for each level. Returns the number of lists.
    static int pos;

    static long parse(String s) {
        long count = 1;
        pos++; // skip '['
        while (s.charAt(pos) == '[') {
            count += parse(s);
        }
        pos++; // skip ']'
        return count;
    }

    public static void main(String[] args) {
        int depth = 1_000_000; // 2 MB of input
        String input = "[".repeat(depth) + "]".repeat(depth);
        pos = 0;
        System.out.println("parsed " + parse(input) + " lists");
    }
}
```

One million levels of nesting use more stack than the thread has. The JVM throws `StackOverflowError`, which is an `Error`, not an `Exception`, so the usual `catch (Exception e)` in a handler does not catch it.

```java run id=depth-limit
public class Main {
    static final int MAX_DEPTH = 64;

    static final class TooDeep extends Exception {
        TooDeep() {
            super("nesting deeper than " + MAX_DEPTH + " levels");
        }
    }

    static int pos;

    static long parse(String s, int depth) throws TooDeep {
        if (depth > MAX_DEPTH) {
            throw new TooDeep();
        }
        long count = 1;
        pos++; // skip '['
        while (s.charAt(pos) == '[') {
            count += parse(s, depth + 1);
        }
        pos++; // skip ']'
        return count;
    }

    static void handle(String input) {
        pos = 0;
        try {
            System.out.println("200 parsed " + parse(input, 1) + " lists");
        } catch (TooDeep e) {
            System.out.println("400 " + e.getMessage());
        }
    }

    public static void main(String[] args) {
        int depth = 1_000_000;
        handle("[".repeat(depth) + "]".repeat(depth));
        handle("[[][[]]]");
    }
}
```

```output for=depth-limit
400 nesting deeper than 64 levels
200 parsed 4 lists
```

The parser counts the depth and stops at 64 levels with a checked exception that the handler turns into a 400 answer.

- Verdict: idiom. A depth counter in the parser is enough. The standard library has no general setting for parser depth.
- `InputStream.readNBytes(int)` reads at most a given number of bytes, so a handler can read the limit plus one byte, as the Python example does.
- `java.util.regex` backtracks. Use possessive quantifiers such as `a++` or atomic groups `(?>...)`, and limit the length of the input first.
- The stack size of a new thread can be set in the `Thread` constructor, but a larger stack only moves the limit. CWE-674 rates a larger stack as a limited fix.
