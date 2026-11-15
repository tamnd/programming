```java test id=account file=AccountTest.java
import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertThrows;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.CsvSource;

class InsufficientFundsException extends RuntimeException {
    InsufficientFundsException(String message) {
        super(message);
    }
}

final class Account {
    private int balance;

    Account(int balance) {
        this.balance = balance;
    }

    int balance() {
        return balance;
    }

    void deposit(int amount) {
        balance += amount;
    }

    void withdraw(int amount) {
        if (amount > balance) {
            throw new InsufficientFundsException("balance " + balance + ", asked " + amount);
        }
        balance -= amount;
    }
}

class AccountTest {
    private Account account;

    // The fixture: JUnit makes a new AccountTest and calls this method before each test.
    @BeforeEach
    void newAccount() {
        account = new Account(100);
    }

    @Test
    void depositAddsToBalance() {
        account.deposit(50);
        assertEquals(150, account.balance());
    }

    @ParameterizedTest
    @CsvSource({"30, 70", "100, 0"})
    void withdrawLeavesTheRest(int amount, int rest) {
        account.withdraw(amount);
        assertEquals(rest, account.balance());
    }

    @Test
    void withdrawMoreThanBalanceFails() {
        assertThrows(InsufficientFundsException.class, () -> account.withdraw(150));
        assertEquals(100, account.balance());
    }
}
```

```output
╷
├─ JUnit Platform Suite ✔
├─ JUnit Jupiter ✔
│  └─ AccountTest ✔
│     ├─ withdrawMoreThanBalanceFails() ✔
│     ├─ depositAddsToBalance() ✔
│     └─ withdrawLeavesTheRest(int, int) ✔
│        ├─ [1] 30, 70 ✔
│        └─ [2] 100, 0 ✔
└─ JUnit Vintage ✔

Test run finished after $TIME
[         5 containers found      ]
[         0 containers skipped    ]
[         5 containers started    ]
[         0 containers aborted    ]
[         5 containers successful ]
[         0 containers failed     ]
[         4 tests found           ]
[         0 tests skipped         ]
[         4 tests started         ]
[         0 tests aborted         ]
[         4 tests successful      ]
[         0 tests failed          ]
```

- Verdict: library. The Java platform has no test framework. JUnit is the usual one; this page runs it with the JUnit console launcher. Maven and Gradle run the same tests in their test phase.
- JUnit finds tests by annotation, not by name. `@Test` marks a test, and `@BeforeEach` marks the fixture method.
- By default, JUnit Jupiter makes a new instance of the test class for each test method, so the field `account` is new in each test.
- `@ParameterizedTest` with `@CsvSource` runs the method once for each row. The report shows each row as its own test under the method.
- `assertThrows` fails when the lambda does not throw an exception of the given class.
