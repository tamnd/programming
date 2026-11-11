```java run id=listeners
import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeSupport;
import java.util.ArrayDeque;
import java.util.Queue;

public class Main {
    static class Thermostat {
        private final PropertyChangeSupport events = new PropertyChangeSupport(this);
        private int target = 20;

        void addListener(PropertyChangeListener listener) {
            events.addPropertyChangeListener(listener);
        }

        void setTarget(int value) {
            int old = target;
            target = value;
            events.firePropertyChange("target", old, value);
        }
    }

    public static void main(String[] args) {
        Queue<Runnable> loop = new ArrayDeque<>();
        Thermostat t = new Thermostat();
        t.addListener(e -> System.out.println(e.getPropertyName() + ": " + e.getOldValue() + " -> " + e.getNewValue()));
        t.addListener(e -> loop.add(() -> System.out.println("later: heater set for " + e.getNewValue())));

        t.setTarget(22);
        t.setTarget(22); // no event: the value did not change
        t.setTarget(18);
        System.out.println("queued: " + loop.size());
        while (!loop.isEmpty()) {
            loop.poll().run();
        }
    }
}
```

```output for=listeners
target: 20 -> 22
target: 22 -> 18
queued: 2
later: heater set for 22
later: heater set for 18
```

- Verdict: stdlib. Java uses listener interfaces for events. `java.beans.PropertyChangeSupport` keeps a list of listeners and calls them when a property changes, and it does not fire when the old and new values are equal. Swing and JavaFX run all user interface handlers on one event thread.
- The second listener does not act at once. It puts a job on a queue, and the loop at the end runs the jobs. This is the split between an event and the work that it causes.
