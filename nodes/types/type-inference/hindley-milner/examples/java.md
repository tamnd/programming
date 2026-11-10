```java run id=var
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class Main {
    public static void main(String[] args) {
        var names = new ArrayList<String>();
        names.add("Ada");
        names.add("Grace");
        var lengths = Map.of("Ada", 3);
        var total = 0L;
        for (var name : names) {
            total += name.length();
        }
        var first = names.get(0);
        System.out.println(names.getClass().getSimpleName() + " " + total + " " + first.toUpperCase() + " " + lengths.get("Ada"));
        System.out.println(((Object) total).getClass().getSimpleName());
    }
}
```

```output for=var
ArrayList 8 ADA 3
Long
```

- Verdict: native. `var` takes the type of its initializer: `ArrayList<String>`, `Map<String, Integer>`, `long` and `String`.
- `var` works for local variables and in `for` loops. Fields, parameters and return types still need written types.
- `var` needs an initializer, and the initializer must not be `null` or a lambda expression (see Pitfalls).
- `var` is a reserved type name, not a keyword, so older code with a variable named `var` still compiles.
