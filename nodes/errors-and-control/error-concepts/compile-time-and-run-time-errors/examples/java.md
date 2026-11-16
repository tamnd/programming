```java run id=when-java
public class Main {
    public static void main(String[] args) {
        int[] items = {10, 20, 30};
        for (int i : new int[] {1, 5}) {
            try {
                System.out.println("items[" + i + "] = " + items[i]);
            } catch (ArrayIndexOutOfBoundsException e) {
                System.out.println("items[" + i + "]: run-time error (" + e.getClass().getSimpleName() + ")");
            }
        }
    }
}
```

```output for=when-java
items[1] = 20
items[5]: run-time error (ArrayIndexOutOfBoundsException)
```

- Verdict: native. The Java compiler finds syntax and type errors, and checks that each checked exception is caught or declared. The run-time system tests each array index, and throws an exception for an index that is not valid.
