```java run id=thread-confinement
import java.util.HashMap;
import java.util.Map;

public class Main {
    public static void main(String[] args) throws InterruptedException {
        String[] words = {"a", "b", "c", "d", "e"};
        int chunks = 4;
        @SuppressWarnings("unchecked")
        Map<String, Integer>[] results = new Map[chunks];
        Thread[] threads = new Thread[chunks];
        for (int i = 0; i < chunks; i++) {
            final int index = i;
            threads[i] = new Thread(() -> {
                Map<String, Integer> counts = new HashMap<>(); // local: no lock
                for (int r = 0; r < 2000; r++) {
                    for (String w : words) counts.merge(w, 1, Integer::sum);
                }
                results[index] = counts; // hand the finished object over
            });
            threads[i].start();
        }
        for (Thread t : threads) t.join(); // after join, main owns the results

        Map<String, Integer> total = new HashMap<>();
        for (Map<String, Integer> counts : results) {
            counts.forEach((w, n) -> total.merge(w, n, Integer::sum));
        }
        int sum = 0;
        for (int n : total.values()) sum += n;
        System.out.println("chunks: " + chunks);
        System.out.println("words counted: " + sum);
        System.out.println("distinct words: " + total.size());
        System.out.println("count of 'a': " + total.get("a"));
    }
}
```

```output
chunks: 4
words counted: 40000
distinct words: 5
count of 'a': 8000
```

- Verdict: idiom. Each thread fills a local `HashMap`, and `join` makes the main thread the only owner of the results.
