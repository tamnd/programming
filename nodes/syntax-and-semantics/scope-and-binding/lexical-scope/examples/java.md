```java run id=scopes
public class Main {
    static String level = "class";

    static String readsField() {
        return level;
    }

    static String caller() {
        String level = "caller";
        return readsField() + ", local " + level;
    }

    public static void main(String[] args) {
        System.out.println(caller());
        int total = 0;
        for (int i = 0; i < 3; i++) {
            int square = i * i;
            total += square;
        }
        System.out.println("total: " + total);
    }
}
```

```output for=scopes
class, local caller
total: 5
```

- Verdict: native. A local variable is in scope from its declaration to the end of its block. `i` and `square` do not exist after the loop.
- A local variable can hide a field, as `level` does in `caller`. A local variable cannot hide another local variable or a parameter of the same method. The compiler rejects it.

```java fails expect=compile id=local-shadow
public class Main {
    public static void main(String[] args) {
        int count = 3;
        for (int i = 0; i < 2; i++) {
            int count = i;
            System.out.println(count);
        }
        System.out.println(count);
    }
}
```

`javac` reports that the variable `count` is already defined in the method. The rule stops a common mistake in C and C++, where an inner declaration silently hides an outer variable.

```java wrong id=self-assign
public class Main {
    static final class Account {
        String owner;

        Account(String owner) {
            owner = owner;
        }
    }

    public static void main(String[] args) {
        System.out.println(new Account("ann").owner);
    }
}
```

```output for=self-assign
null
```

In the constructor, the parameter `owner` hides the field `owner`. Both sides of `owner = owner` name the parameter, so the field keeps its default value `null`. Write `this.owner = owner`.
