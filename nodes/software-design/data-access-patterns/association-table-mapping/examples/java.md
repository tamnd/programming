```java run id=link-table
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import java.util.TreeSet;

public class Main {
    record Link(long employeeId, long skillId) implements Comparable<Link> {
        public int compareTo(Link o) {
            int c = Long.compare(employeeId, o.employeeId);
            return c != 0 ? c : Long.compare(skillId, o.skillId);
        }
    }

    // A stand-in for the database: two entity tables and a link table of key pairs.
    static final Map<Long, String> EMPLOYEE = new TreeMap<>();
    static final Map<Long, String> SKILL = new TreeMap<>(Map.of(1L, "Python", 2L, "SQL", 3L, "Go"));
    static final TreeSet<Link> EMPLOYEE_SKILL = new TreeSet<>(); // Sorted by both keys, like a primary key.
    static long lastEmployee = 0;

    static void insertLink(long employeeId, long skillId) {
        if (!EMPLOYEE_SKILL.add(new Link(employeeId, skillId))) {
            throw new IllegalStateException("UNIQUE constraint failed: employee_skill.employee_id, employee_skill.skill_id");
        }
    }

    static class Employee {
        Long id;
        String name;
        List<Long> skills; // A list of skill keys.

        Employee(String name, List<Long> skills) {
            this.name = name;
            this.skills = skills;
        }
    }

    static void insert(Employee e) {
        EMPLOYEE.put(++lastEmployee, e.name);
        e.id = lastEmployee;
        for (long s : e.skills) {
            insertLink(e.id, s);
        }
    }

    static int[] updateSkills(Employee e) {
        // Compare the collection with the link rows: insert the new pairs, delete the old ones.
        List<Long> inDb = new ArrayList<>();
        for (Link l : EMPLOYEE_SKILL) {
            if (l.employeeId() == e.id) {
                inDb.add(l.skillId());
            }
        }
        List<Long> added = new ArrayList<>();
        for (long s : e.skills) {
            if (!inDb.contains(s)) {
                added.add(s);
            }
        }
        List<Long> removed = new ArrayList<>();
        for (long s : inDb) {
            if (!e.skills.contains(s)) {
                removed.add(s);
            }
        }
        for (long s : added) {
            insertLink(e.id, s);
        }
        for (long s : removed) {
            EMPLOYEE_SKILL.remove(new Link(e.id, s));
        }
        return new int[] {removed.size(), added.size()};
    }

    static String skillsOf(long employeeId) {
        List<String> names = new ArrayList<>();
        for (Link l : EMPLOYEE_SKILL) {
            if (l.employeeId() == employeeId) {
                names.add(SKILL.get(l.skillId()));
            }
        }
        return String.join(", ", names);
    }

    static String employeesWith(long skillId) {
        List<String> names = new ArrayList<>();
        for (Link l : EMPLOYEE_SKILL) {
            if (l.skillId() == skillId) {
                names.add(EMPLOYEE.get(l.employeeId()));
            }
        }
        return String.join(", ", names);
    }

    static String links() {
        List<String> parts = new ArrayList<>();
        for (Link l : EMPLOYEE_SKILL) {
            parts.add(l.employeeId() + "-" + l.skillId());
        }
        return String.join(" ", parts);
    }

    public static void main(String[] args) {
        Employee ann = new Employee("Ann", List.of(1L, 2L));
        insert(ann);
        insert(new Employee("Bo", List.of(1L)));
        System.out.println("links: " + links());
        System.out.println("Ann: " + skillsOf(1));
        System.out.println("Python: " + employeesWith(1));

        ann.skills = List.of(2L, 3L);
        int[] counts = updateSkills(ann);
        System.out.println("update Ann: deleted " + counts[0] + ", inserted " + counts[1]);
        System.out.println("links: " + links());
        System.out.println("Ann: " + skillsOf(1));

        try {
            insertLink(2, 1);
        } catch (IllegalStateException ex) {
            System.out.println("link 2-1 again: " + ex.getMessage());
        }
    }
}
```

```output
links: 1-1 1-2 2-1
Ann: Python, SQL
Python: Ann, Bo
update Ann: deleted 1, inserted 1
links: 1-2 1-3 2-1
Ann: SQL, Go
link 2-1 again: UNIQUE constraint failed: employee_skill.employee_id, employee_skill.skill_id
```

- Verdict: idiom. A link row is a `record` that is `Comparable`, and the link table is a `TreeSet` of these records, so the rows iterate in key order and `add` returns `false` for a pair that is already there.
- With JPA or Hibernate, `@ManyToMany` and `@JoinTable` declare the same link table, and the provider writes the link rows.
