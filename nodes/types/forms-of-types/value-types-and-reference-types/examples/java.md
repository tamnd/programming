```java run id=call-by-sharing
import java.util.ArrayList;
import java.util.List;

public class Main {
    static void addItem(List<String> list) {
        list.add("b"); // changes the object that the caller also sees
    }

    static void replace(List<String> list) {
        list = new ArrayList<>(); // rebinds only the parameter
        list.add("z");
    }

    static void bump(int n) {
        n++; // changes only the copy
    }

    public static void main(String[] args) {
        List<String> items = new ArrayList<>(List.of("a"));
        addItem(items);
        replace(items);
        System.out.println(items);

        int count = 1;
        bump(count);
        System.out.println(count);

        List<String> other = new ArrayList<>(List.of("a", "b"));
        System.out.println((items == other) + " " + items.equals(other));
    }
}
```

```output for=call-by-sharing
[a, b]
1
false true
```

- Verdict: native. A `List` is a reference type, so `addItem` gets a copy of the reference and changes the caller's list. `replace` assigns another list to its parameter, and the caller does not see that. An `int` is a primitive type, so `bump` changes only its copy. `==` compares references, and `equals` compares the contents.
