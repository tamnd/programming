```java run id=stream-decorate
import java.util.Comparator;
import java.util.List;

public class Main {
    static int calls;

    static int vowels(String w) {
        calls++;
        return (int) w.chars().filter(c -> "aeiou".indexOf(c) >= 0).count();
    }

    record Decorated(int key, String word) {}

    public static void main(String[] args) {
        List<String> words = List.of("banana", "kiwi", "apple", "fig", "cherry", "date", "plum", "mango");

        // comparingInt calls the key function twice at each comparison.
        calls = 0;
        List<String> slow = words.stream().sorted(Comparator.comparingInt(Main::vowels)).toList();
        System.out.println(slow + " " + (calls > words.size()));

        // Decorate with map, sort by the stored key, undecorate with map.
        calls = 0;
        List<String> fast = words.stream()
                .map(w -> new Decorated(vowels(w), w))
                .sorted(Comparator.comparingInt(Decorated::key))
                .map(Decorated::word)
                .toList();
        System.out.println(fast + " " + calls);
    }
}
```

```output
[fig, cherry, plum, kiwi, apple, date, mango, banana] true
[fig, cherry, plum, kiwi, apple, date, mango, banana] 8
```

`Comparator.comparingInt` calls the key function at each comparison. The stream `map` to a `Decorated` record computes each key once, `sorted` compares the stored keys, and a second `map` keeps the words. The stream sort is stable, because the stream from a `List` is ordered.
