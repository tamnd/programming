```java run id=fall-through-java
public class Main {
    static String colon(int level) {
        StringBuilder out = new StringBuilder();
        switch (level) {
            case 3:
                out.append(" delete");
            case 2:
                out.append(" write");
            case 1:
                out.append(" read");
                break;
            default:
                out.append(" none");
        }
        return out.toString();
    }

    static String arrow(int level) {
        StringBuilder out = new StringBuilder();
        switch (level) {
            case 3 -> out.append(" delete");
            case 2 -> out.append(" write");
            case 1 -> out.append(" read");
            default -> out.append(" none");
        }
        return out.toString();
    }

    public static void main(String[] args) {
        System.out.println("colon 3:" + colon(3));
        System.out.println("arrow 3:" + arrow(3));
    }
}
```

```output for=fall-through-java
colon 3: delete write read
arrow 3: delete
```

- Verdict: native. A switch with `case ... :` labels falls through, as in C. A switch with `case ... ->` labels runs only the code of the matching rule, so `arrow(3)` gives only ` delete`.
