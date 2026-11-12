```java run id=comparable-mixin
interface Ordered<T> {
    int compare(T other); // required

    default boolean lessThan(T other) { // provided
        return compare(other) < 0;
    }

    @SuppressWarnings("unchecked")
    default boolean between(T lo, T hi) {
        return ((Ordered<T>) lo).compare((T) this) <= 0 && compare(hi) <= 0;
    }
}

record Version(int major, int minor) implements Ordered<Version> {
    public int compare(Version o) {
        return major != o.major ? Integer.compare(major, o.major) : Integer.compare(minor, o.minor);
    }

    public String toString() { return major + "." + minor; }
}

public class Main {
    static String yes(boolean flag) { return flag ? "yes" : "no"; }

    public static void main(String[] args) {
        Version v12 = new Version(1, 2), v110 = new Version(1, 10), v20 = new Version(2, 0);
        System.out.println(v12 + " < " + v110 + ": " + yes(v12.lessThan(v110)));
        System.out.println(v110 + " between " + v12 + " and " + v20 + ": " + yes(v110.between(v12, v20)));
        System.out.println(v20 + " between " + v12 + " and " + v110 + ": " + yes(v20.between(v12, v110)));
    }
}
```

```output
1.2 < 1.10: yes
1.10 between 1.2 and 2.0: yes
2.0 between 1.2 and 1.10: no
```

- Verdict: idiom. An interface with default methods is a mixin with no state. A class can implement many such interfaces, and a record can too.
