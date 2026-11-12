```java run id=multiton
import java.util.Map;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.CountDownLatch;

public class Main {
    static final class Currency {
        private static final Map<String, Integer> DECIMALS =
                Map.of("USD", 2, "EUR", 2, "JPY", 0); // the valid keys
        private static final ConcurrentHashMap<String, Currency> INSTANCES =
                new ConcurrentHashMap<>();

        final String code;
        final int decimals;

        private Currency(String code, int decimals) {
            this.code = code;
            this.decimals = decimals;
        }

        static Currency of(String code) {
            Integer d = DECIMALS.get(code);
            if (d == null) throw new IllegalArgumentException(code);
            // one instance per key; the function runs at most once per key
            return INSTANCES.computeIfAbsent(code, c -> new Currency(c, d));
        }

        static int count() {
            return INSTANCES.size();
        }
    }

    public static void main(String[] args) throws InterruptedException {
        System.out.println("USD has " + Currency.of("USD").decimals + " decimals");
        System.out.println("JPY has " + Currency.of("JPY").decimals + " decimals");
        System.out.println("same USD object: " + (Currency.of("USD") == Currency.of("USD")));
        System.out.println("instances: " + Currency.count());
        try {
            Currency.of("XYZ");
        } catch (IllegalArgumentException e) {
            System.out.println(e.getMessage() + ": unknown currency");
        }

        Set<Currency> seen = ConcurrentHashMap.newKeySet();
        CountDownLatch start = new CountDownLatch(1);
        Thread[] threads = new Thread[8];
        for (int i = 0; i < threads.length; i++) {
            threads[i] = new Thread(() -> {
                try {
                    start.await(); // all eight threads ask at the same time
                } catch (InterruptedException e) {
                    return;
                }
                seen.add(Currency.of("EUR"));
            });
            threads[i].start();
        }
        start.countDown();
        for (Thread t : threads) t.join();
        System.out.println("8 threads ask for EUR: " + seen.size() + " object");
        System.out.println("instances: " + Currency.count());
    }
}
```

```output
USD has 2 decimals
JPY has 0 decimals
same USD object: true
instances: 2
XYZ: unknown currency
8 threads ask for EUR: 1 object
instances: 3
```

- Verdict: stdlib. `ConcurrentHashMap.computeIfAbsent` performs the whole call atomically, so two threads cannot both create an instance for one key, and the registry needs no lock in the code.
- `java.util.Currency` is a multiton in the standard library: there is never more than one `Currency` instance for a currency, and the class has no public constructor.
- When the keys are fixed at compile time, an `enum` is the simpler form. The language creates one instance per constant.
