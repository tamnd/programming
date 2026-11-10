```java run id=sensor
import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeSupport;

public class Main {
    static class Sensor {
        private final PropertyChangeSupport support = new PropertyChangeSupport(this);
        private double value;

        void subscribe(PropertyChangeListener listener) {
            support.addPropertyChangeListener("value", listener);
        }

        void unsubscribe(PropertyChangeListener listener) {
            support.removePropertyChangeListener("value", listener);
        }

        void set(double next) {
            double old = value;
            value = next;
            support.firePropertyChange("value", old, next);
        }
    }

    public static void main(String[] args) {
        Sensor sensor = new Sensor();
        PropertyChangeListener display =
                event -> System.out.printf("display: %.1f%n", (Double) event.getNewValue());
        PropertyChangeListener alarm = event -> {
            double value = (Double) event.getNewValue();
            if (value > 30) {
                System.out.printf("alarm: %.1f is above 30%n", value);
            }
        };
        sensor.subscribe(display);
        sensor.subscribe(alarm);
        sensor.set(21.5);
        sensor.set(30.5);
        sensor.unsubscribe(display);
        sensor.set(31.0);
        sensor.set(31.0);
    }
}
```

```output for=sensor
display: 21.5
display: 30.5
alarm: 30.5 is above 30
alarm: 31.0 is above 30
```

- Verdict: stdlib. `java.beans.PropertyChangeSupport` keeps the listeners and fires a `PropertyChangeEvent` that carries the property name, the old value and the changed value. It is in the `java.desktop` module.
- `PropertyChangeListener` has one method, so a lambda can be a listener. Removal needs the same listener object, so the program keeps it in a variable.
- The last call `set(31.0)` prints nothing. `firePropertyChange` fires no event when the old and the changed values are equal and not null.
- `java.util.Observable` and `java.util.Observer` are deprecated. `java.util.concurrent.Flow` holds the interfaces of reactive streams for asynchronous notification.
