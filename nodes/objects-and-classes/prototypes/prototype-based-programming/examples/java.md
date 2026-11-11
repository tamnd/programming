```java run id=delegating-map
import java.util.HashMap;
import java.util.Map;
import java.util.function.Function;

// A prototype object: own slots plus a parent to delegate to.
final class Proto {
    final Proto parent;
    final Map<String, Object> slots = new HashMap<>();

    Proto(Proto parent) { this.parent = parent; }

    Object get(String name) {
        for (Proto o = this; o != null; o = o.parent) {
            if (o.slots.containsKey(name)) return o.slots.get(name);
        }
        throw new IllegalArgumentException("no slot " + name);
    }

    Proto set(String name, Object value) { slots.put(name, value); return this; }

    @SuppressWarnings("unchecked")
    Object send(String name) { return ((Function<Proto, Object>) get(name)).apply(this); }

    Proto cloneProto() {
        Proto c = new Proto(parent);
        c.slots.putAll(slots);
        return c;
    }
}

public class Main {
    public static void main(String[] args) {
        Proto traits = new Proto(null);
        traits.set("describe", (Function<Proto, Object>) o -> "(" + o.get("x") + ", " + o.get("y") + ")");
        Proto origin = new Proto(traits).set("x", 0).set("y", 0);

        Proto p = origin.cloneProto().set("x", 3);
        System.out.println(origin.send("describe") + " " + p.send("describe"));

        traits.set("describe", (Function<Proto, Object>) o -> "<" + o.get("x") + " " + o.get("y") + ">");
        System.out.println(origin.send("describe") + " " + p.send("describe"));
    }
}
```

```output for=delegating-map
(0, 0) (3, 0)
<0 0> <3 0>
```

```java fails expect=exception:CloneNotSupportedException id=not-cloneable
// The prototype pattern with Object.clone, on a class that forgot Cloneable.
class Shape {
    int x, y;

    Shape copy() throws CloneNotSupportedException {
        return (Shape) super.clone();  // Object.clone checks for Cloneable at run time
    }
}

public class Main {
    public static void main(String[] args) throws Exception {
        Shape prototype = new Shape();
        prototype.x = 1;
        Shape s = prototype.copy();
        System.out.println(s.x);
    }
}
```

- Verdict: absent. Java objects get their fields and methods from their class, and the set of fields is fixed at compile time. A program that needs objects that delegate to other objects builds them from a map of slots and a parent reference, as the run block does, with functions as methods and casts on every read. The prototype design pattern, which makes new objects by copying a configured one, is the common form of the idea in Java code.
- The failing block uses `Object.clone` for the prototype pattern on a class that does not implement `Cloneable`. The method compiles, because `clone` is declared on `Object`, but it throws `CloneNotSupportedException` at run time. A copy constructor or a copy method that builds the new object field by field avoids the check, and makes the depth of the copy explicit.
