```java wrong id=format
public class Main {
    static String commentHtml(String author, String text) {
        return String.format("<p class=\"comment\"><b>%s</b>: %s</p>", author, text);
    }

    public static void main(String[] args) {
        System.out.println(commentHtml("alice", "Nice post."));
        System.out.println(commentHtml("mallory", "<script>alert(1)</script>"));
    }
}
```

```output for=format
<p class="comment"><b>alice</b>: Nice post.</p>
<p class="comment"><b>mallory</b>: <script>alert(1)</script></p>
```

```java run id=escape
public class Main {
    // Java SE has no HTML escape function. This one covers HTML text and
    // quoted attribute values, and nothing else.
    static String escapeHtml(String s) {
        StringBuilder out = new StringBuilder(s.length());
        for (char c : s.toCharArray()) {
            switch (c) {
                case '&' -> out.append("&amp;");
                case '<' -> out.append("&lt;");
                case '>' -> out.append("&gt;");
                case '"' -> out.append("&quot;");
                case '\'' -> out.append("&#x27;");
                default -> out.append(c);
            }
        }
        return out.toString();
    }

    static String commentHtml(String author, String text) {
        return String.format("<p class=\"comment\"><b>%s</b>: %s</p>",
                escapeHtml(author), escapeHtml(text));
    }

    public static void main(String[] args) {
        System.out.println(commentHtml("alice", "Nice post."));
        System.out.println(commentHtml("mallory", "<script>alert(1)</script>"));
    }
}
```

```output for=escape
<p class="comment"><b>alice</b>: Nice post.</p>
<p class="comment"><b>mallory</b>: &lt;script&gt;alert(1)&lt;/script&gt;</p>
```

- Verdict: library. Java SE has no HTML escape function and no HTML template engine. The function in the second example covers the five characters of the OWASP table for HTML text. In real code, use a maintained encoder, such as the OWASP Java Encoder, and pick the encoder for the context of each value.
- A template engine that escapes by default removes most of the work. Many engines also have a form that does not escape. Review each use of it.
- A servlet that writes input to `response.getWriter()` has the weakness. CodeQL query `java/xss` follows input from a request to such a writer.
