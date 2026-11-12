```java run id=owner-decides
import java.util.ArrayList;
import java.util.List;

public class Main {
    static final class Team {
        static final int MAX = 3;
        private final List<String> members = new ArrayList<>();

        List<String> members() { return List.copyOf(members); } // an unmodifiable copy

        void addMember(String name) {
            if (members.contains(name)) return;
            if (members.size() >= MAX) throw new IllegalStateException("team is full");
            members.add(name);
        }

        void removeMember(String name) { members.remove(name); }
    }

    public static void main(String[] args) {
        Team team = new Team();
        for (String n : List.of("ana", "bo", "cy")) team.addMember(n);
        try {
            team.members().add("dee");
            throw new AssertionError("expected an exception");
        } catch (UnsupportedOperationException e) {
            System.out.println("the copy is unmodifiable");
        }
        team.removeMember("bo");
        team.addMember("dee");
        if (!team.members().equals(List.of("ana", "cy", "dee"))) throw new AssertionError(team.members());
        System.out.println(team.members());
        System.out.println("encapsulate collection: tests pass");
    }
}
```

```output
the copy is unmodifiable
[ana, cy, dee]
encapsulate collection: tests pass
```
