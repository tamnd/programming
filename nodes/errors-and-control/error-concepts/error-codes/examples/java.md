```java run id=codes-java
public class Main {
    static final int OK = 0, EMPTY = 1, NOT_A_NUMBER = 2, OUT_OF_RANGE = 3;
    static final String[] NAMES = {"ok", "empty", "not a number", "out of range"};

    // Returns a code. On success, writes the port to out[0].
    static int parsePort(String text, int[] out) {
        if (text.isEmpty()) return EMPTY;
        if (!text.matches("[0-9]+")) return NOT_A_NUMBER;
        if (text.length() > 5 || Integer.parseInt(text) > 65535) return OUT_OF_RANGE;
        out[0] = Integer.parseInt(text);
        return OK;
    }

    public static void main(String[] args) {
        for (String text : new String[] {"8080", "", "x", "99999"}) {
            int[] port = new int[1];
            int code = parsePort(text, port);
            if (code != OK) {
                System.out.println("'" + text + "' -> error code " + code + " (" + NAMES[code] + ")");
            } else {
                System.out.println("'" + text + "' -> port " + port[0]);
            }
        }
    }
}
```

```output for=codes-java
'8080' -> port 8080
'' -> error code 1 (empty)
'x' -> error code 2 (not a number)
'99999' -> error code 3 (out of range)
```

- Verdict: idiom. Java reports errors with exceptions, and its tutorial names the separation of error-handling code from regular code as the first advantage of exceptions. A method has one return value, so this one writes the port to an array of one element.
