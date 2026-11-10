```java run id=spawner
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

public class Main {
    interface Prototype {
        Prototype copy();

        String describe();
    }

    static final class Monster implements Prototype {
        final String name;
        final int health;
        final List<String> items;

        Monster(String name, int health, List<String> items) {
            this.name = name;
            this.health = health;
            this.items = new ArrayList<>(items);
        }

        // A copy constructor: it copies the list, so the copy has its own items.
        Monster(Monster other) {
            this(other.name, other.health, other.items);
        }

        @Override
        public Monster copy() {
            return new Monster(this);
        }

        @Override
        public String describe() {
            return name + ": health " + health + ", items " + String.join(", ", items);
        }
    }

    record Trap(String name, int damage) implements Prototype {
        // A record is immutable here, so the copy can be the same object.
        @Override
        public Trap copy() {
            return this;
        }

        @Override
        public String describe() {
            return name + ": damage " + damage;
        }
    }

    static final Map<String, Prototype> registry = new LinkedHashMap<>();

    static Prototype spawn(String key) {
        return registry.get(key).copy();
    }

    public static void main(String[] args) {
        registry.put("goblin", new Monster("goblin", 30, List.of("dagger")));
        registry.put("spikes", new Trap("spikes", 5));

        Monster first = (Monster) spawn("goblin");
        first.items.add("torch");
        Prototype second = spawn("goblin");
        Prototype trap = spawn("spikes");
        for (Prototype thing : List.of(first, second, trap)) {
            System.out.println(thing.describe());
        }
        System.out.println("prototype " + registry.get("goblin").describe());
    }
}
```

```output for=spawner
goblin: health 30, items dagger, torch
goblin: health 30, items dagger
spikes: damage 5
prototype goblin: health 30, items dagger
```

- Verdict: idiom. An interface with a `copy` method, implemented with a copy constructor. The return type of `copy` in `Monster` is `Monster`, a covariant return type, so a caller that knows the class needs no cast.
- The program does not use `Object.clone`. `clone` is `protected` in `Object`, the `Cloneable` interface has no methods, and `Object.clone` makes a shallow copy that skips the constructors. The node page shows why a `final` field that needs a deep copy cannot work with `clone`.
- `Trap` is a record whose components are immutable, so it returns itself. An immutable prototype needs no copy at all.
- Arrays implement `Cloneable`, and `array.clone()` returns an array of the same type. It is shallow: an array of arrays shares the inner arrays.
