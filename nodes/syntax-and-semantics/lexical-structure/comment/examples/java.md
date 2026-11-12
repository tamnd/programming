```java run id=unicode-escape-in-comment
public class Main {
    /**
     * Returns the larger of two ints. Javadoc reads comments that start with two stars.
     */
    static int max(int a, int b) {
        return a > b ? a : b;
    }

    public static void main(String[] args) {
        System.out.println(max(3, 7));

        // The compiler turns unicode escapes into characters before it finds comments,
        // so the escape for a line break ends this comment: \u000a System.out.println("This line runs.");

        /* Block comments do not nest, so a star-slash inside one ends it early. */
        String s = "/* inside a string, this is not a comment */";
        System.out.println(s.length());
    }
}
```

```output for=unicode-escape-in-comment
7
This line runs.
44
```

- Verdict: native. Java has `//`, `/* */` and the Javadoc form `/** */`. The compiler translates Unicode escapes before it finds comments, so `\u000a` in a line comment is a real line break, and the statement after it runs. Comment markers inside a string literal are text, so the string has 44 characters.
