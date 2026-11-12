```java run id=let-the-code-say-it
import java.util.List;

public class Main {
    record Parcel(int grams) {}

    // Before: comments explain what each block does.
    static int shippingCentsBefore(List<Parcel> parcels, int zone) {
        // weight must be positive for every parcel
        // compute the total weight in grams
        int grams = 0;
        for (Parcel p : parcels) grams += p.grams();
        // round up to whole kilograms
        int kg = (grams + 999) / 1000;
        // look up the rate for the zone (1 = domestic, 2 = europe, other = world)
        int rate = zone == 1 ? 450 : zone == 2 ? 900 : 1600;
        // the carrier charges at least 2 kg per shipment, see contract section 4
        return Math.max(kg, 2) * rate;
    }

    // After: names and a check say what; one comment says why.
    enum Zone {
        DOMESTIC(450), EUROPE(900), WORLD(1600);

        final int centsPerKg;
        Zone(int centsPerKg) { this.centsPerKg = centsPerKg; }

        static Zone of(int code) { return code == 1 ? DOMESTIC : code == 2 ? EUROPE : WORLD; }
    }

    /** Returns the weight to charge for, in whole kilograms, with the carrier's minimum applied. */
    static int billableKilograms(List<Parcel> parcels) {
        if (!parcels.stream().allMatch(p -> p.grams() > 0)) {
            throw new IllegalArgumentException("every parcel must have a positive weight");
        }
        int grams = parcels.stream().mapToInt(Parcel::grams).sum();
        int kg = Math.ceilDiv(grams, 1000);
        // The carrier charges at least 2 kg per shipment (contract, section 4).
        return Math.max(kg, 2);
    }

    static int shippingCents(List<Parcel> parcels, Zone zone) {
        return billableKilograms(parcels) * zone.centsPerKg;
    }

    static void check(boolean ok, String what) {
        if (!ok) throw new AssertionError(what);
    }

    public static void main(String[] args) {
        List<Parcel> parcels = List.of(new Parcel(700), new Parcel(1450));
        for (int code : new int[] {1, 2, 3}) {
            check(shippingCents(parcels, Zone.of(code)) == shippingCentsBefore(parcels, code), "zone " + code);
        }
        check(shippingCents(List.of(new Parcel(300)), Zone.DOMESTIC) == 900, "minimum");
        check(shippingCents(parcels, Zone.EUROPE) == 2700, "europe");
        try {
            shippingCents(List.of(new Parcel(0)), Zone.DOMESTIC);
            throw new AssertionError("the check must fail");
        } catch (IllegalArgumentException expected) {
        }
        System.out.println("shipping: tests pass");
    }
}
```

```output
shipping: tests pass
```

- The `enum` replaces the comment that listed the zone codes. Each zone has a name and a rate, and a caller cannot pass a zone that does not exist.
- Java's `assert` statement is disabled unless the JVM runs with `-ea`, so this example uses an exception for the check, which always runs.
- `Math.ceilDiv` (Java 18) rounds up, which replaces the `(grams + 999) / 1000` idiom and the comment that explained it.
