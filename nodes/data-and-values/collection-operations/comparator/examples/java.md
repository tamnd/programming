```java run id=comparing-and-overflow
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.List;

public class Main {
    record Person(String name, int age) {}

    public static void main(String[] args) {
        List<Person> people = List.of(
                new Person("cy", 31), new Person("ann", 31), new Person("bob", 25));

        // Comparator.comparing makes a comparator from a key; thenComparing adds a second key.
        Comparator<Person> byAgeThenName =
                Comparator.comparingInt(Person::age).thenComparing(Person::name);
        System.out.println(people.stream().sorted(byAgeThenName).map(Person::name).toList());

        // reversed() turns the whole comparator around.
        System.out.println(people.stream().sorted(byAgeThenName.reversed()).map(Person::name).toList());

        // A lambda can implement compare directly; Integer.compare gives -1, 0 or 1.
        Comparator<Person> byAge = (a, b) -> Integer.compare(a.age(), b.age());
        System.out.println(byAge.compare(people.get(0), people.get(2)));

        // Subtraction overflows: MIN_VALUE - 1 wraps to MAX_VALUE, so the order is wrong.
        Integer[] nums = {1, Integer.MIN_VALUE, 0};
        Arrays.sort(nums, (a, b) -> a - b);
        System.out.println(Arrays.toString(nums));
        Arrays.sort(nums, Integer::compare);
        System.out.println(Arrays.toString(nums));

        // nullsFirst wraps a comparator so that null items do not throw.
        List<String> names = new ArrayList<>(Arrays.asList("b", null, "a"));
        names.sort(Comparator.nullsFirst(Comparator.naturalOrder()));
        System.out.println(names);
    }
}
```

```output
[bob, ann, cy]
[cy, ann, bob]
1
[0, 1, -2147483648]
[-2147483648, 0, 1]
[null, a, b]
```

- Verdict: native. `comparingInt` and `thenComparing` build a comparator from two keys, and `reversed` turns it around. `Integer.compare` gives -1, 0 or 1. The comparator `a - b` overflows for `Integer.MIN_VALUE - 1`, so the first sort puts `MIN_VALUE` last. `nullsFirst` puts `null` before the other items instead of throwing `NullPointerException`.
