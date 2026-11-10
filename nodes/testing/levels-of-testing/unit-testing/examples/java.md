```java norun reason=fragment id=cart file=CartTest.java
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import java.util.List;
import org.junit.jupiter.api.Test;

record Item(int price, int quantity) {}

final class Cart {
    private Cart() {}

    static int total(List<Item> items) {
        int total = 0;
        for (Item item : items) {
            if (item.quantity() < 0) {
                throw new IllegalArgumentException("negative quantity: " + item.quantity());
            }
            total += item.price() * item.quantity();
        }
        return total;
    }
}

class CartTest {
    @Test
    void emptyCartIsZero() {
        assertEquals(0, Cart.total(List.of()));
    }

    @Test
    void sumsPriceTimesQuantity() {
        assertEquals(600, Cart.total(List.of(new Item(250, 2), new Item(100, 1))));
    }

    @Test
    void rejectsNegativeQuantity() {
        assertThrows(IllegalArgumentException.class, () -> Cart.total(List.of(new Item(250, -1))));
    }
}
```

The JUnit Platform console launcher, run with `--details=tree` on the compiled class, prints:

```text
╷
├─ JUnit Platform Suite ✔
├─ JUnit Jupiter ✔
│  └─ CartTest ✔
│     ├─ emptyCartIsZero() ✔
│     ├─ rejectsNegativeQuantity() ✔
│     └─ sumsPriceTimesQuantity() ✔
└─ JUnit Vintage ✔

Test run finished after 218 ms
[         4 containers found      ]
[         0 containers skipped    ]
[         4 containers started    ]
[         0 containers aborted    ]
[         4 containers successful ]
[         0 containers failed     ]
[         3 tests found           ]
[         0 tests skipped         ]
[         3 tests started         ]
[         0 tests aborted         ]
[         3 tests successful      ]
[         0 tests failed          ]

```

- Verdict: library. The Java standard library has no test framework. JUnit is the usual one. Its programming model, JUnit Jupiter, is in the `org.junit.jupiter` packages. Maven and Gradle run it in their test phase, and the console launcher runs it from the command line.
- A test is a method with the `@Test` annotation. The test class and its methods need not be public.
- `assertThrows` runs the lambda and fails when it does not throw an exception of the given class. It returns the exception, so a test can check its message.
- The Java `assert` statement is not a replacement: the JVM skips it unless it runs with the `-ea` option.
- This block does not run on this page, because it needs the JUnit library and its launcher, and the runner of this site has the standard library only.
