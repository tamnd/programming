```java run id=say-the-assumption
public class Main {
    static long refundCents(long paidCents, int daysPaid, int daysUsed) {
        assert daysUsed >= 0 && daysUsed <= daysPaid : "days used must be between 0 and days paid";
        return Math.round((double) paidCents * (daysPaid - daysUsed) / daysPaid);
    }

    public static void main(String[] args) {
        boolean enabled = false;
        assert enabled = true; // the assignment runs only when assertions are enabled
        System.out.println("assertions enabled: " + enabled);

        if (refundCents(3000, 30, 10) != 2000) throw new IllegalStateException("refund");
        if (!enabled) {
            // Without assertions, the broken assumption is not caught.
            if (refundCents(3000, 30, 31) != -100) throw new IllegalStateException("expected -100");
        }
        System.out.println("introduce assertion: tests pass");
    }
}
```

```output
assertions enabled: false
introduce assertion: tests pass
```
