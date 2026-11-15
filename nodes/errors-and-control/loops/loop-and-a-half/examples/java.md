```java run id=read-lines-with-break-and-in-the-condition
import java.io.BufferedReader;
import java.io.IOException;
import java.io.StringReader;

public class Main {
    public static void main(String[] args) throws IOException {
        // Read, test, process: the exit is in the middle.
        BufferedReader in = new BufferedReader(new StringReader("3\n4\n\n5\n"));
        int sum = 0;
        while (true) {
            String line = in.readLine();
            if (line == null || line.isEmpty()) {
                break;
            }
            sum += Integer.parseInt(line);
        }
        System.out.println("sum before the blank line: " + sum);

        // The read moves into the condition with an assignment.
        BufferedReader in2 = new BufferedReader(new StringReader("ab\ncde\n"));
        String line;
        int count = 0;
        while ((line = in2.readLine()) != null) {
            count += line.length();
        }
        System.out.println("characters: " + count);
    }
}
```

```output for=read-lines-with-break-and-in-the-condition
sum before the blank line: 7
characters: 5
```

- Verdict: idiom. The first loop declares `line` inside the body and leaves with `break` at the end of the input or at a blank line, so the 5 after the blank line is not added. The second loop puts the read into the condition, which needs `line` to be declared before the loop.
