```java run id=overload-override
import java.util.List;

public class Main {
    static class Animal {
        String sound() { return "..."; }
    }

    static class Cat extends Animal {
        @Override
        String sound() { return "meow"; }
    }

    // Overloads: the compiler picks one by the declared type of the argument.
    static String describe(Object o) { return "object"; }

    static String describe(String s) { return "string"; }

    // Generics: one body for every element type.
    static <T> T first(List<T> items) { return items.get(0); }

    public static void main(String[] args) {
        Animal a = new Cat();
        System.out.println(a.sound()); // the object picks the override

        Object o = "hi";
        System.out.println(describe(o) + " " + describe("hi"));

        System.out.println(first(List.of(3, 4)) + " " + first(List.of("a", "b")));
    }
}
```

```output for=overload-override
meow
object string
3 a
```

- Verdict: native. The variable `a` has the declared type `Animal`, but the object is a `Cat`, so the call runs `Cat.sound`. `describe(o)` calls the `Object` overload, although `o` holds a string, because the compiler chooses an overload by the declared type `Object`. `first` is a generic method, and the compiler infers `T` for each call.
