```java run id=one-table-for-hierarchy
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

public class Main {
    // One table for the whole hierarchy. A missing value is null.
    record Row(String type, String name, String club, Integer runs, Integer wickets) {}

    static final Map<Long, Row> PLAYER = new TreeMap<>();
    static long lastId = 0;

    abstract static class Player {
        Long id;
        final String name;

        Player(String name) {
            this.name = name;
        }

        abstract String describe();
    }

    static class Footballer extends Player {
        final String club;

        Footballer(String name, String club) {
            super(name);
            this.club = club;
        }

        String describe() {
            return "Footballer " + name + ", club " + club;
        }
    }

    static class Cricketer extends Player {
        final int runs;

        Cricketer(String name, int runs) {
            super(name);
            this.runs = runs;
        }

        String describe() {
            return "Cricketer " + name + ", runs " + runs;
        }
    }

    static class Bowler extends Cricketer {
        final int wickets;

        Bowler(String name, int runs, int wickets) {
            super(name, runs);
            this.wickets = wickets;
        }

        @Override
        String describe() {
            return "Bowler " + name + ", runs " + runs + ", wickets " + wickets;
        }
    }

    static final Map<String, List<String>> SUBTYPES = Map.of("cricketer", List.of("cricketer", "bowler"));

    static void insert(Player p) {
        // Each class writes its own fields; the columns of the other classes stay null.
        Row row;
        if (p instanceof Bowler b) {
            row = new Row("bowler", b.name, null, b.runs, b.wickets);
        } else if (p instanceof Cricketer c) {
            row = new Row("cricketer", c.name, null, c.runs, null);
        } else {
            Footballer f = (Footballer) p;
            row = new Row("footballer", f.name, f.club, null, null);
        }
        p.id = ++lastId;
        PLAYER.put(p.id, row);
    }

    static Player load(long id, Row r) {
        Player p = switch (r.type()) {
            case "footballer" -> new Footballer(r.name(), r.club());
            case "cricketer" -> new Cricketer(r.name(), r.runs());
            default -> new Bowler(r.name(), r.runs(), r.wickets());
        };
        p.id = id;
        return p;
    }

    static List<Player> findAll(String kind) {
        List<String> kinds = kind == null ? null : SUBTYPES.getOrDefault(kind, List.of(kind));
        List<Player> result = new ArrayList<>();
        for (var e : PLAYER.entrySet()) {
            if (kinds == null || kinds.contains(e.getValue().type())) {
                result.add(load(e.getKey(), e.getValue()));
            }
        }
        return result;
    }

    static String show(Object v) {
        return v == null ? "null" : v.toString();
    }

    public static void main(String[] args) {
        insert(new Footballer("Ann", "Lions"));
        insert(new Cricketer("Bo", 412));
        insert(new Bowler("Cy", 95, 31));
        for (var e : PLAYER.entrySet()) {
            Row r = e.getValue();
            System.out.println("row: " + e.getKey() + " " + r.type() + " " + r.name() + " "
                    + show(r.club()) + " " + show(r.runs()) + " " + show(r.wickets()));
        }
        for (Player p : findAll(null)) {
            System.out.println("loaded: " + p.describe());
        }
        List<String> names = new ArrayList<>();
        for (Player p : findAll("cricketer")) {
            names.add(p.name);
        }
        System.out.println("cricketers: " + String.join(", ", names));
        int nulls = 0;
        for (Row r : PLAYER.values()) {
            nulls += (r.club() == null ? 1 : 0) + (r.runs() == null ? 1 : 0) + (r.wickets() == null ? 1 : 0);
        }
        System.out.println("null cells in subclass columns: " + nulls + " of " + 3 * PLAYER.size());
    }
}
```

```output
row: 1 footballer Ann Lions null null
row: 2 cricketer Bo null 412 null
row: 3 bowler Cy null 95 31
loaded: Footballer Ann, club Lions
loaded: Cricketer Bo, runs 412
loaded: Bowler Cy, runs 95, wickets 31
cricketers: Bo, Cy
null cells in subclass columns: 5 of 9
```

- Verdict: idiom. A row is a record with boxed `Integer` fields, so `null` can stand for an unused column. `insert` uses pattern matching for `instanceof` and tests `Bowler` first, and `load` uses a `switch` on the type code.
- With JPA or Hibernate, `@Inheritance(strategy = InheritanceType.SINGLE_TABLE)` on the root class and `@DiscriminatorValue` on each subclass give the same mapping.
