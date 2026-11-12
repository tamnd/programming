```java run id=initializer-and-labeled-blocks
public class Main {
    static int count;

    // A static initializer block runs once, when the class is initialized.
    static {
        count = 10;
    }

    public static void main(String[] args) {
        int x = 1;
        {
            int y = x + 1;  // y is local to this block
            System.out.println("y = " + y);
        }
        // int x = 5; in the block above would not compile, because a local cannot hide a local

        // A labeled block can be left with break.
        found: {
            for (int i = 0; i < 5; i++) {
                if (i * i > 6) {
                    System.out.println("first i with i*i > 6: " + i);
                    break found;
                }
            }
            System.out.println("not found");
        }

        // try-with-resources closes the resource at the end of the block.
        try (AutoCloseable r = () -> System.out.println("closed")) {
            System.out.println("in try, count = " + count);
        } catch (Exception e) {
            System.out.println(e);
        }

        // A switch expression arm can be a block that yields a value.
        String s = switch (x) {
            case 1 -> {
                String t = "one";
                yield t.toUpperCase();
            }
            default -> "other";
        };
        System.out.println("s = " + s);
    }
}
```

```output for=initializer-and-labeled-blocks
y = 2
first i with i*i > 6: 3
in try, count = 10
closed
s = ONE
```

- Verdict: native. A static initializer block runs once when the class is initialized. A labeled block can be left with `break`. `try` with resources closes the resource at the end of the block. An arm of a `switch` expression can be a block that gives its value with `yield`. A local variable cannot hide another local variable.
