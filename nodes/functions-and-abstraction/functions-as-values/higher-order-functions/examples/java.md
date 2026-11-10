```java run id=hof
import java.util.Comparator;
import java.util.List;
import java.util.function.BinaryOperator;
import java.util.function.Function;

public class Main {
    static <T> T twice(Function<T, T> f, T x) {
        return f.apply(f.apply(x));
    }

    public static void main(String[] args) {
        List<String> words = List.of("pear", "fig", "banana");
        System.out.println(twice(n -> n * 3, 2));
        System.out.println(words.stream().map(String::length).toList());
        System.out.println(words.stream().filter(w -> w.length() > 3).toList());
        BinaryOperator<Integer> add = Integer::sum;
        System.out.println(List.of(1, 2, 3, 4).stream().reduce(0, add));
        System.out.println(words.stream().sorted(Comparator.comparing(String::length)).toList());
        Function<Integer, Integer> addOne = n -> n + 1;
        Function<Integer, Integer> addThenDouble = addOne.andThen(n -> n * 2);
        System.out.println(addThenDouble.apply(5));
    }
}
```

```output for=hof
18
[4, 3, 6]
[pear, banana]
10
[fig, pear, banana]
12
```

- Verdict: native. A lambda expression or a method reference such as `String::length` makes an object of a functional interface: an interface with one abstract method.
- The package `java.util.function` has the common shapes: `Function`, `Predicate`, `Supplier`, `Consumer` and `BinaryOperator`. A call goes through the method of the interface, here `apply`.
- `Function` has the default methods `andThen` and `compose`, which return a composed function.
- The type parameters of these interfaces take reference types, so `Function<Integer, Integer>` boxes each `int`. Primitive shapes such as `IntUnaryOperator` avoid the boxing.
