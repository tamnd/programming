```java run id=clone-and-copy-constructor
import java.util.ArrayList;
import java.util.List;

public class Main {
    static final class Team implements Cloneable {
        String name;
        List<String> members;

        Team(String name, List<String> members) {
            this.name = name;
            this.members = members;
        }

        // A copy constructor: it chooses a deep copy for the list.
        Team(Team other) {
            this(other.name, new ArrayList<>(other.members));
        }

        // Object.clone copies the fields; the list is shared.
        @Override
        public Team clone() {
            try {
                return (Team) super.clone();
            } catch (CloneNotSupportedException e) {
                throw new AssertionError(e);
            }
        }
    }

    public static void main(String[] args) {
        Team a = new Team("red", new ArrayList<>(List.of("Ann", "Bo")));
        Team shallow = a.clone();
        Team deep = new Team(a);

        a.members.add("Cy");
        System.out.println(shallow.members);
        System.out.println(deep.members);
        System.out.println(shallow != a);
        System.out.println(shallow.members == a.members);
    }
}
```

```output for=clone-and-copy-constructor
[Ann, Bo, Cy]
[Ann, Bo]
true
true
```

- Verdict: native. `Object.clone` is protected; a class implements `Cloneable` and overrides `clone` as a public method that calls `super.clone()`.
- `super.clone()` copies each field, so the clone is a new object with the same list.
- The copy constructor is ordinary code, so it can copy the list. It needs no cast and no checked exception.
