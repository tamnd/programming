```java run id=booking-system
import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public class Main {
    record Flight(int id, String city) {}

    static final class Booking {
        final int flightId;
        final String customer;
        boolean sent;

        Booking(int flightId, String customer) {
            this.flightId = flightId;
            this.customer = customer;
        }
    }

    // The store stands in for the database. Its methods are synchronized, so each call is
    // one transaction, and two users cannot both take the last seat.
    static final class Store {
        private final Flight flight = new Flight(7, "Oslo");
        private int seatsLeft = 1;
        private final List<Booking> bookings = new ArrayList<>();

        synchronized String book(int flightId, String customer) {
            if (flightId != flight.id() || seatsLeft == 0) return "no seat left";
            seatsLeft--;
            bookings.add(new Booking(flightId, customer));
            return "ok";
        }

        // Many screens: each screen shows the same data in a different way.
        synchronized String agentScreen(int flightId) {
            String names = bookings.stream().filter(b -> b.flightId == flightId).map(b -> b.customer)
                    .collect(Collectors.joining(", "));
            return "agent screen: flight " + flightId + " to " + flight.city() + ", seats left " + seatsLeft
                    + ", booked by " + names;
        }

        synchronized String customerScreen(String customer) {
            String trips = bookings.stream().filter(b -> b.customer.equals(customer))
                    .map(b -> "flight " + b.flightId + " to " + flight.city()).collect(Collectors.joining("; "));
            return "customer screen for " + customer + ": " + trips;
        }

        // Batch processing and integration: a night job sends new bookings to another system as a file.
        synchronized List<String> nightBatch() {
            List<String> lines = new ArrayList<>();
            for (Booking b : bookings) {
                if (!b.sent) {
                    lines.add(b.flightId + ";" + b.customer);
                    b.sent = true;
                }
            }
            return lines;
        }
    }

    public static void main(String[] args) {
        Store store = new Store();
        for (String customer : List.of("Ann", "Bob")) {
            System.out.println(customer + " books flight 7: " + store.book(7, customer));
        }
        System.out.println(store.agentScreen(7));
        System.out.println(store.customerScreen("Ann"));
        List<String> lines = store.nightBatch();
        System.out.println("night batch: " + lines.size() + " booking sent to the airline system");
        System.out.println("airline file: " + String.join("\n", lines));
        System.out.println("second night batch: " + store.nightBatch().size() + " bookings");
    }
}
```

```output
Ann books flight 7: ok
Bob books flight 7: no seat left
agent screen: flight 7 to Oslo, seats left 0, booked by Ann
customer screen for Ann: flight 7 to Oslo
night batch: 1 booking sent to the airline system
airline file: 7;Ann
second night batch: 0 bookings
```

A `Store` object stands in for the database. Its methods are `synchronized`, so each call runs alone, as a database transaction would. A real Java application would use JDBC or JPA, and the transaction would be in the database.
