```java run id=equals-without-hashcode
import java.util.HashSet;
import java.util.Objects;
import java.util.Set;

public class Main {
    static final class NoHash {
        final String id;
        NoHash(String id) { this.id = id; }

        @Override
        public boolean equals(Object o) {
            return o instanceof NoHash n && id.equals(n.id);
        }
    }

    static final class WithHash {
        final String id;
        WithHash(String id) { this.id = id; }

        @Override
        public boolean equals(Object o) {
            return o instanceof WithHash w && id.equals(w.id);
        }

        @Override
        public int hashCode() {
            return Objects.hash(id);
        }
    }

    public static void main(String[] args) {
        Set<NoHash> a = new HashSet<>();
        a.add(new NoHash("x"));
        System.out.println(new NoHash("x").equals(new NoHash("x")) + " " + a.contains(new NoHash("x")));

        Set<WithHash> b = new HashSet<>();
        b.add(new WithHash("x"));
        b.add(new WithHash("x"));
        System.out.println(b.contains(new WithHash("x")) + " " + b.size());
    }
}
```

```output for=equals-without-hashcode
true false
true 1
```

- Verdict: native. `Object` gives `equals` and `hashCode`, and the contract of `hashCode` says that equal objects must produce the same integer.
- `NoHash` breaks the contract: two equal objects keep the identity hash of `Object`, so `HashSet.contains` looks in the wrong bucket and returns false.
- `WithHash` hashes the same field that `equals` compares, and the set keeps one copy.
