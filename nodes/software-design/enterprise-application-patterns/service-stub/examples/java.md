```java run id=tax-service-stub
import java.util.List;
import java.util.Map;

public class Main {
    interface TaxService {
        long rateBp(String region); // the rate in basis points: 650 is 6.5%
    }

    static class ServiceUnavailable extends RuntimeException {
        ServiceUnavailable(String m) { super(m); }
    }

    // The real service. Here it stands for a remote call that is not reachable.
    static class RemoteTaxService implements TaxService {
        public long rateBp(String region) {
            throw new ServiceUnavailable("the tax service is not reachable");
        }
    }

    // Runs locally, fast and in memory, with canned rates.
    static class TaxServiceStub implements TaxService {
        static final Map<String, Long> RATES = Map.of("north", 650L, "south", 0L);
        int calls = 0;

        public long rateBp(String region) {
            calls++;
            return RATES.getOrDefault(region, 500L); // a default rate for the other regions
        }
    }

    static class BillingService {
        private final TaxService taxes;
        BillingService(TaxService taxes) { this.taxes = taxes; }

        long totalCents(long amountCents, String region) {
            return amountCents + amountCents * taxes.rateBp(region) / 10000;
        }
    }

    static String money(long c) { return String.format("%d.%02d", c / 100, c % 100); }

    public static void main(String[] args) {
        try {
            new BillingService(new RemoteTaxService()).totalCents(10000, "north");
        } catch (ServiceUnavailable e) {
            System.out.println("real service: error: " + e.getMessage());
        }

        TaxServiceStub stub = new TaxServiceStub();
        BillingService billing = new BillingService(stub);
        for (String region : List.of("north", "south", "east")) {
            System.out.println(region + ": 100.00 with tax is " + money(billing.totalCents(10000, region)));
        }
        System.out.println("the stub got " + stub.calls + " calls");
    }
}
```

```output
real service: error: the tax service is not reachable
north: 100.00 with tax is 106.50
south: 100.00 with tax is 100.00
east: 100.00 with tax is 105.00
the stub got 3 calls
```

- Verdict: idiom. `TaxService` is an interface, and the stub and the real service implement it. Mocking libraries such as Mockito can make such a stub at run time, but a small class like this one is easy to read.
