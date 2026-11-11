```java run id=sharing
import java.util.ArrayList;
import java.util.List;

public class Main {
    static void update(List<Integer> items, int count) {
        items.add(4);
        items = new ArrayList<>();
        count++;
    }

    static int sum(int... values) {
        int total = 0;
        for (int v : values) {
            total += v;
        }
        return total;
    }

    public static void main(String[] args) {
        List<Integer> numbers = new ArrayList<>(List.of(1, 2, 3));
        int count = 1;
        update(numbers, count);
        System.out.println(numbers + " " + count);
        System.out.println(sum() + " " + sum(1, 2, 3));
    }
}
```

```output for=sharing
[1, 2, 3, 4] 1
0 6
```

- Verdict: native. Java passes all arguments by value. The values of the argument expressions initialize new parameter variables. For a reference type, the value is a reference, so the method can change the object but not the variable of the caller.
- A varargs parameter `int... values` receives the extra arguments as an array.
- Java has no default values and no named arguments. Overloads and builder objects are the usual replacements.

```java wrong id=swap
public class Main {
    static void swap(Integer a, Integer b) {
        Integer t = a;
        a = b;
        b = t;
    }

    public static void main(String[] args) {
        Integer x = 1;
        Integer y = 2;
        swap(x, y);
        System.out.println(x + " " + y);
    }
}
```

```output for=swap
1 2
```

The method swaps its own copies of the two references. Java has no way to pass a variable by reference.
