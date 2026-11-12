```java run id=consultants
import java.util.ArrayList;
import java.util.List;
import java.util.function.Consumer;

public class Main {
    static final class Signal {
        private final List<Consumer<Consultant>> handlers = new ArrayList<>();
        void add(Consumer<Consultant> h) { handlers.add(h); }
        void fire(Consultant c) { handlers.forEach(h -> h.accept(c)); }
    }

    static final class Consultant {
        final Signal locationChanged = new Signal();
        final Signal availabilityChanged = new Signal();
        final String name, home;
        final boolean availableAtHome;
        String location;

        Consultant(String name, String home, boolean availableAtHome, String location) {
            this.name = name;
            this.home = home;
            this.availableAtHome = availableAtHome;
            this.location = location;
        }

        boolean availableAt(String place) { return place.equals(home) ? availableAtHome : true; }

        void move(String place) {
            System.out.println(name + " moves to " + place);
            if (!location.equals(place)) {
                locationChanged.fire(this);
                if (availableAt(place) != availableAt(location)) availabilityChanged.fire(this);
                location = place;
            }
        }
    }

    static final class EventAggregator {
        final Signal changed = new Signal();
        final Signal location = new Signal();
        final Signal availability = new Signal();

        void listen(Consultant c) {
            c.locationChanged.add(x -> signal(location, x));
            c.availabilityChanged.add(x -> signal(availability, x));
        }

        private void signal(Signal specific, Consultant c) {
            specific.fire(c); // the specific event
            changed.fire(c); // and the general one
        }
    }

    public static void main(String[] args) {
        EventAggregator agg = new EventAggregator();
        Consultant alice = new Consultant("alice", "London", false, "London");
        Consultant bob = new Consultant("bob", "Paris", true, "Lyon");
        agg.listen(alice);
        agg.listen(bob);

        agg.changed.add(c -> System.out.println("screen: " + c.name + " changed"));
        agg.availability.add(c -> System.out.println("phone: availability of " + c.name + " changed"));

        alice.move("Leeds");
        bob.move("Paris");
    }
}
```

```output for=consultants
alice moves to Leeds
screen: alice changed
phone: availability of alice changed
screen: alice changed
bob moves to Paris
screen: bob changed
```

- Verdict: idiom. Lists of `Consumer` functions are the events.
