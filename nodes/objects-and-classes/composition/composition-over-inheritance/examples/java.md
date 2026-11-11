```java run id=forwarding-class
import java.util.Collection;
import java.util.Comparator;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;

// The part that the class needs from a set, as an interface.
interface Bag<E> {
    boolean add(E e);
    boolean addAll(Collection<? extends E> c);
    int size();
}

// Composition: CountingSet has-a Set and forwards to it.
final class CountingSet<E> implements Bag<E> {
    private final Set<E> inner;
    private int added = 0;

    CountingSet(Set<E> inner) { this.inner = inner; }

    public boolean add(E e) {
        added++;
        return inner.add(e);
    }

    public boolean addAll(Collection<? extends E> c) {
        added += c.size();
        return inner.addAll(c); // calls the inner set, never this.add
    }

    public int size() { return inner.size(); }

    int added() { return added; }

    @Override public String toString() { return inner.toString(); }
}

public class Main {
    public static void main(String[] args) {
        var s = new CountingSet<String>(new HashSet<>());
        s.add("a");
        s.addAll(List.of("b", "c"));
        System.out.println(s.added() + " " + s.size());

        // Any Set works as the part, chosen when the object is made.
        var t = new CountingSet<String>(new TreeSet<>(Comparator.reverseOrder()));
        t.addAll(List.of("a", "b", "c"));
        System.out.println(t.added() + " " + t);
    }
}
```

```output for=forwarding-class
3 3
3 [c, b, a]
```

```java wrong id=fragile-subclass
import java.util.Collection;
import java.util.HashSet;
import java.util.List;

// Inheritance: count the elements that callers try to add.
class CountingHashSet<E> extends HashSet<E> {
    int added = 0;

    @Override public boolean add(E e) {
        added++;
        return super.add(e);
    }

    @Override public boolean addAll(Collection<? extends E> c) {
        added += c.size();
        return super.addAll(c); // HashSet.addAll calls this.add for each element
    }
}

public class Main {
    public static void main(String[] args) {
        var s = new CountingHashSet<String>();
        s.addAll(List.of("a", "b", "c"));
        System.out.println(s.added + " " + s.size());
    }
}
```

```output for=fragile-subclass
6 3
```

- Verdict: idiom. `CountingSet` implements a small interface and forwards each call to a `Set` that it receives in its constructor. Because the class depends only on the `Set` interface, a `HashSet`, a `TreeSet` in reverse order or a test double can be the part. Java has no syntax for delegation, so each forwarded method is written by hand; a large interface such as `Set` needs a forwarding class with every method.
- The wrong block extends `HashSet` and counts in both `add` and `addAll`. `HashSet.addAll` comes from `AbstractCollection`, which calls `add` for each element, so each element counts twice and the count is 6. The subclass depends on an implementation detail of its base class. Declare classes `final` unless they are designed and documented for extension.
