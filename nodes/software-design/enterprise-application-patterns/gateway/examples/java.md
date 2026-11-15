```java run id=shipping-gateway
import java.util.Map;

public class Main {
    // The foreign API of a shipping service. Its names and types do not fit our code:
    // the weight is in grams, the flags are a string, and the result is a map
    // with a return code and the amount as a string of cents.
    interface Connection {
        Map<String, String> quote(long weightGrams, String dest, String svcFlags);
    }

    static Map<String, String> foreignQuote(long weightGrams, String dest, String svcFlags) {
        System.out.println("  connection: quote(weight_grams=" + weightGrams + ", dest=" + dest + ", svc_flags=" + svcFlags + ")");
        if (weightGrams > 30000) return Map.of("rc", "17", "msg", "too heavy");
        return Map.of("rc", "0", "amt", String.valueOf(500 + weightGrams / 100 * 10), "cur", "EUR");
    }

    static class NoService extends Exception {
        NoService(String message) { super(message); }
    }

    record Cost(long cents, String currency) {}

    // Our interface to the shipping service, in the terms of our code.
    static class ShippingGateway {
        private final Connection connection; // the call to the foreign API; a test can replace it

        ShippingGateway(Connection connection) { this.connection = connection; }

        Cost standardCost(double kilograms, String country) throws NoService {
            Map<String, String> reply = connection.quote(Math.round(kilograms * 1000), country, "STD");
            if (!reply.get("rc").equals("0")) throw new NoService("code " + reply.get("rc") + ": " + reply.get("msg"));
            return new Cost(Long.parseLong(reply.get("amt")), reply.get("cur"));
        }
    }

    static void show(ShippingGateway gateway, double kilograms, String country) {
        String kg = String.format("%.1f", kilograms);
        try {
            Cost c = gateway.standardCost(kilograms, country);
            System.out.printf("parcel of %s kg to %s costs %d.%02d %s%n", kg, country, c.cents() / 100, c.cents() % 100, c.currency());
        } catch (NoService e) {
            System.out.println("parcel of " + kg + " kg to " + country + ": no service (" + e.getMessage() + ")");
        }
    }

    public static void main(String[] args) {
        ShippingGateway gateway = new ShippingGateway(Main::foreignQuote);
        show(gateway, 1.2, "DE");
        show(gateway, 31.0, "DE");

        // A test gives the gateway a connection that returns canned data.
        ShippingGateway canned = new ShippingGateway((g, d, f) -> Map.of("rc", "0", "amt", "999", "cur", "EUR"));
        show(canned, 2.0, "FR");
    }
}
```

```output
  connection: quote(weight_grams=1200, dest=DE, svc_flags=STD)
parcel of 1.2 kg to DE costs 6.20 EUR
  connection: quote(weight_grams=31000, dest=DE, svc_flags=STD)
parcel of 31.0 kg to DE: no service (code 17: too heavy)
parcel of 2.0 kg to FR costs 9.99 EUR
```

- Verdict: idiom. The `Connection` interface is the call to the foreign API, so a method reference or a lambda can be the connection. The gateway returns a `Cost` record (Java 16 or later) and throws the checked exception `NoService`.
